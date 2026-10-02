/*
 * SPDX-FileCopyrightText: Paranoid Android
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "camhidl_shim"

#include <aidl/vendor/qti/hardware/camera/postproc/BnPostProcService.h>
#include <aidl/vendor/qti/hardware/camera/postproc/BnPostProcServiceCallBacks.h>
#include <aidl/vendor/qti/hardware/camera/postproc/BnPostProcSession.h>
#include <aidlcommonsupport/NativeHandle.h>
#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <VendorTagDescriptor.h>
#include <android/hardware/graphics/mapper/4.0/IMapper.h>
#include <vendor/qti/hardware/camera/postproc/1.0/IPostProcService.h>

#include <dlfcn.h>

#include <cstring>
#include <mutex>
#include <string>

using android::sp;
using android::status_t;
using android::hardware::hidl_handle;
using android::hardware::hidl_vec;
using android::hardware::Return;
using android::hardware::Void;
using android::hidl::base::V1_0::IBase;

namespace aidlpp = aidl::vendor::qti::hardware::camera::postproc;
namespace hidlpp = vendor::qti::hardware::camera::postproc::V1_0;

// The stock postproc service is HIDL and lives in the camera provider, where
// CamX registers it. Its clients are the NtCam nodes in the same process and
// the NtCam service in another one. Keep the HIDL object in-process and expose
// it to other processes as the AIDL vendor.qti.hardware.camera.postproc HAL;
// remote HIDL lookups are answered with an adapter over that AIDL service.
namespace {

const std::string kAidlInstance =
        std::string(aidlpp::IPostProcService::descriptor) + "/camerapostprocservice";

std::mutex gLock;
sp<hidlpp::IPostProcService> gLocalPostProc;

std::vector<aidlpp::BufferParams> toAidl(const hidl_vec<hidlpp::BufferParams>& in) {
    std::vector<aidlpp::BufferParams> out;
    for (const auto& b : in) {
        out.push_back({.format = static_cast<int32_t>(b.format),
                       .width = static_cast<int32_t>(b.width),
                       .height = static_cast<int32_t>(b.height)});
    }
    return out;
}

hidl_vec<hidlpp::BufferParams> toHidl(const std::vector<aidlpp::BufferParams>& in) {
    hidl_vec<hidlpp::BufferParams> out(in.size());
    for (size_t i = 0; i < in.size(); i++) {
        out[i] = {.format = static_cast<uint32_t>(in[i].format),
                  .width = static_cast<uint32_t>(in[i].width),
                  .height = static_cast<uint32_t>(in[i].height)};
    }
    return out;
}

aidlpp::PostProcCapabilities toAidl(const hidlpp::PostProcCapabilities& in) {
    const auto& j = in.jpegStream;
    aidlpp::PostProcCapabilities out;
    out.jpegStream.maxStreamsSupported = j.maxStreamsSupported;
    out.jpegStream.maxResoultion = {.width = static_cast<int32_t>(j.maxResoultion.width),
                                    .height = static_cast<int32_t>(j.maxResoultion.height)};
    out.jpegStream.minResolution = {.width = static_cast<int32_t>(j.minResolution.width),
                                    .height = static_cast<int32_t>(j.minResolution.height)};
    out.jpegStream.formats.assign(j.formats.begin(), j.formats.end());
    return out;
}

hidlpp::PostProcCapabilities toHidl(const aidlpp::PostProcCapabilities& in) {
    const auto& j = in.jpegStream;
    hidlpp::PostProcCapabilities out;
    out.jpegStream.maxStreamsSupported = j.maxStreamsSupported;
    out.jpegStream.maxResoultion = {.width = static_cast<uint32_t>(j.maxResoultion.width),
                                    .height = static_cast<uint32_t>(j.maxResoultion.height)};
    out.jpegStream.minResolution = {.width = static_cast<uint32_t>(j.minResolution.width),
                                    .height = static_cast<uint32_t>(j.minResolution.height)};
    out.jpegStream.formats = std::vector<uint32_t>(j.formats.begin(), j.formats.end());
    return out;
}

// Nothing's PostProcResult has a size field for non-JPEG results next to the
// JPEG one; only the one matching the type is meaningful, so it rides in
// jpegResult.frameSize.
aidlpp::PostProcResult toAidl(const hidlpp::PostProcResult& in) {
    aidlpp::PostProcResult out;
    out.requestId = in.requestId;
    out.streamId = in.streamId;
    out.postProcTypeVal = static_cast<aidlpp::PostProcType>(in.postProcTypeVal);
    out.jpegResult.frameSize = in.postProcTypeVal == hidlpp::PostProcType::JPEG
                                       ? in.jpegResult.frameSize
                                       : in.resultSize;
    return out;
}

hidlpp::PostProcResult toHidl(const aidlpp::PostProcResult& in) {
    hidlpp::PostProcResult out = {};
    out.requestId = in.requestId;
    out.streamId = in.streamId;
    out.postProcTypeVal = static_cast<hidlpp::PostProcType>(in.postProcTypeVal);
    if (out.postProcTypeVal == hidlpp::PostProcType::JPEG) {
        out.jpegResult.frameSize = in.jpegResult.frameSize;
    } else {
        out.resultSize = in.jpegResult.frameSize;
    }
    return out;
}

// Nothing's ProcessRequestParams carries several metadata buffers plus frame
// number, sequence id and type, which the AIDL parcelable lacks. Pack them in
// front of the metadata bytes: frameNum, sequenceId, type, count, sizes, data.
std::vector<uint8_t> packMetadata(const hidlpp::ProcessRequestParams& in) {
    std::vector<uint32_t> header = {in.frameNum, in.sequenceId,
                                    static_cast<uint32_t>(in.postProcTypeVal),
                                    static_cast<uint32_t>(in.metadata.size())};
    for (const auto& m : in.metadata) header.push_back(m.size());

    std::vector<uint8_t> out(header.size() * sizeof(uint32_t));
    memcpy(out.data(), header.data(), out.size());
    for (const auto& m : in.metadata) out.insert(out.end(), m.begin(), m.end());
    return out;
}

bool unpackMetadata(const std::vector<uint8_t>& in, hidlpp::ProcessRequestParams* out) {
    auto readU32 = [&](size_t& pos, uint32_t* v) {
        if (pos + sizeof(uint32_t) > in.size()) return false;
        memcpy(v, in.data() + pos, sizeof(uint32_t));
        pos += sizeof(uint32_t);
        return true;
    };

    size_t pos = 0;
    uint32_t type, count;
    if (!readU32(pos, &out->frameNum) || !readU32(pos, &out->sequenceId) ||
        !readU32(pos, &type) || !readU32(pos, &count)) {
        return false;
    }
    out->postProcTypeVal = static_cast<hidlpp::PostProcType>(type);

    std::vector<uint32_t> sizes(count);
    for (auto& size : sizes) {
        if (!readU32(pos, &size)) return false;
    }

    out->metadata.resize(count);
    for (uint32_t i = 0; i < count; i++) {
        if (sizes[i] > in.size() - pos) return false;
        out->metadata[i] = std::vector<uint8_t>(in.begin() + pos, in.begin() + pos + sizes[i]);
        pos += sizes[i];
    }
    return true;
}

/* Provider side: AIDL service over the in-process HIDL implementation. */

class HidlCallbacks : public hidlpp::IPostProcServiceCallBacks {
  public:
    explicit HidlCallbacks(std::shared_ptr<aidlpp::IPostProcServiceCallBacks> cb)
        : mCb(std::move(cb)) {}

    Return<void> notifyResult(hidlpp::Error error, const hidlpp::PostProcResult& result) override {
        mCb->notifyResult(static_cast<aidlpp::Error>(error), toAidl(result));
        return Void();
    }

    std::shared_ptr<aidlpp::IPostProcServiceCallBacks> mCb;
};

class AidlSession : public aidlpp::BnPostProcSession {
  public:
    AidlSession(sp<hidlpp::IPostProcSession> session, sp<HidlCallbacks> cb)
        : mSession(std::move(session)), mCb(std::move(cb)) {}

    ~AidlSession() override { mSession->release(); }

    ndk::ScopedAStatus abort() override {
        mSession->abort();
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus process(const aidlpp::ProcessRequestParams& params) override {
        std::vector<native_handle_t*> handles;
        auto convert = [&](const std::vector<aidlpp::HandleParams>& in) {
            hidl_vec<hidlpp::HandleParams> out(in.size());
            for (size_t i = 0; i < in.size(); i++) {
                native_handle_t* nh = android::makeFromAidl(in[i].bufHandle);
                handles.push_back(nh);
                out[i].format = in[i].format;
                out[i].width = in[i].width;
                out[i].height = in[i].height;
                out[i].bufHandle = hidl_handle(nh);
            }
            return out;
        };

        hidlpp::ProcessRequestParams hparams;
        hparams.input = convert(params.input);
        hparams.output = convert(params.output);
        hparams.streamId = params.streamId;
        if (!unpackMetadata(params.metadata, &hparams)) {
            for (auto* nh : handles) native_handle_delete(nh);
            mCb->mCb->notifyRequestId(aidlpp::Error::DEVICE_BAD_STATE, 0);
            return ndk::ScopedAStatus::ok();
        }

        uint32_t requestId = 0;
        hidlpp::Error error = hidlpp::Error::POSTPROC_FAIL;
        mSession->process(hparams, [&](uint32_t id, hidlpp::Error err) {
            requestId = id;
            error = err;
        });

        // The FDs still belong to the AIDL parcel.
        for (auto* nh : handles) native_handle_delete(nh);

        mCb->mCb->notifyRequestId(static_cast<aidlpp::Error>(error), requestId);
        return ndk::ScopedAStatus::ok();
    }

  private:
    sp<hidlpp::IPostProcSession> mSession;
    sp<HidlCallbacks> mCb;
};

class AidlService : public aidlpp::BnPostProcService {
  public:
    explicit AidlService(sp<hidlpp::IPostProcService> service) : mService(std::move(service)) {}

    ndk::ScopedAStatus createPostProcessor(
            const aidlpp::CreateParams& params,
            const std::shared_ptr<aidlpp::IPostProcServiceCallBacks>& callback,
            std::shared_ptr<aidlpp::IPostProcSession>* session) override {
        hidlpp::CreateParams hparams;
        hparams.postProcTypeVal = static_cast<hidlpp::PostProcType>(params.postProcTypeVal);
        hparams.input = toHidl(params.input);
        hparams.output = toHidl(params.output);

        sp<HidlCallbacks> cb = new HidlCallbacks(callback);
        sp<hidlpp::IPostProcSession> hsession = mService->createPostProcessor(hparams, cb);
        *session = hsession ? ndk::SharedRefBase::make<AidlSession>(hsession, cb) : nullptr;
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus getCapabilities(aidlpp::PostProcType type,
                                       aidlpp::PostProcCapabilities* caps) override {
        mService->getCapabilities(static_cast<hidlpp::PostProcType>(type),
                                  [&](const hidlpp::PostProcCapabilities& c) { *caps = toAidl(c); });
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus getPostprocTypes(std::vector<aidlpp::PostProcType>* types) override {
        mService->getPostprocTypes([&](const hidl_vec<hidlpp::PostProcType>& t) {
            for (auto v : t) types->push_back(static_cast<aidlpp::PostProcType>(v));
        });
        return ndk::ScopedAStatus::ok();
    }

  private:
    sp<hidlpp::IPostProcService> mService;
};

/* Client side: HIDL interface over the remote AIDL service. */

class AidlCallbacks : public aidlpp::BnPostProcServiceCallBacks {
  public:
    explicit AidlCallbacks(sp<hidlpp::IPostProcServiceCallBacks> cb) : mCb(std::move(cb)) {}

    ndk::ScopedAStatus notifyResult(aidlpp::Error error,
                                    const aidlpp::PostProcResult& result) override {
        mCb->notifyResult(static_cast<hidlpp::Error>(error), toHidl(result));
        return ndk::ScopedAStatus::ok();
    }

    // Delivered from within IPostProcSession::process() on the same call chain.
    ndk::ScopedAStatus notifyRequestId(aidlpp::Error error, int32_t reqId) override {
        std::lock_guard<std::mutex> lock(mLock);
        mError = static_cast<hidlpp::Error>(error);
        mRequestId = reqId;
        return ndk::ScopedAStatus::ok();
    }

    std::mutex mLock;
    hidlpp::Error mError = hidlpp::Error::POSTPROC_FAIL;
    uint32_t mRequestId = 0;

  private:
    sp<hidlpp::IPostProcServiceCallBacks> mCb;
};

class HidlSession : public hidlpp::IPostProcSession {
  public:
    HidlSession(std::shared_ptr<aidlpp::IPostProcSession> session,
                std::shared_ptr<AidlCallbacks> cb)
        : mSession(std::move(session)), mCb(std::move(cb)) {}

    Return<void> process(const hidlpp::ProcessRequestParams& params, process_cb _hidl_cb) override {
        std::lock_guard<std::mutex> lock(mProcessLock);
        auto session = getSession();
        if (!session) {
            _hidl_cb(0, hidlpp::Error::SESSION_NOT_INIT);
            return Void();
        }

        auto convert = [](const hidl_vec<hidlpp::HandleParams>& in) {
            std::vector<aidlpp::HandleParams> out(in.size());
            for (size_t i = 0; i < in.size(); i++) {
                out[i].format = in[i].format;
                out[i].width = in[i].width;
                out[i].height = in[i].height;
                out[i].bufHandle = android::dupToAidl(in[i].bufHandle.getNativeHandle());
            }
            return out;
        };

        aidlpp::ProcessRequestParams aparams;
        aparams.input = convert(params.input);
        aparams.output = convert(params.output);
        aparams.streamId = params.streamId;
        aparams.metadata = packMetadata(params);

        {
            std::lock_guard<std::mutex> cbLock(mCb->mLock);
            mCb->mError = hidlpp::Error::POSTPROC_FAIL;
            mCb->mRequestId = 0;
        }
        auto status = session->process(aparams);

        std::lock_guard<std::mutex> cbLock(mCb->mLock);
        _hidl_cb(mCb->mRequestId,
                 status.isOk() ? mCb->mError : hidlpp::Error::DEVICE_BAD_STATE);
        return Void();
    }

    Return<hidlpp::Error> abort() override {
        auto session = getSession();
        if (!session) return hidlpp::Error::SESSION_NOT_INIT;
        return session->abort().isOk() ? hidlpp::Error::NONE : hidlpp::Error::DEVICE_BAD_STATE;
    }

    // Dropping the last reference releases the session on the provider side.
    Return<hidlpp::Error> release() override {
        std::lock_guard<std::mutex> lock(mSessionLock);
        mSession.reset();
        return hidlpp::Error::NONE;
    }

  private:
    std::shared_ptr<aidlpp::IPostProcSession> getSession() {
        std::lock_guard<std::mutex> lock(mSessionLock);
        return mSession;
    }

    std::mutex mProcessLock;
    std::mutex mSessionLock;
    std::shared_ptr<aidlpp::IPostProcSession> mSession;
    std::shared_ptr<AidlCallbacks> mCb;
};

class HidlService : public hidlpp::IPostProcService {
  public:
    explicit HidlService(std::shared_ptr<aidlpp::IPostProcService> service)
        : mService(std::move(service)) {}

    Return<void> getPostprocTypes(getPostprocTypes_cb _hidl_cb) override {
        std::vector<aidlpp::PostProcType> types;
        mService->getPostprocTypes(&types);
        hidl_vec<hidlpp::PostProcType> out(types.size());
        for (size_t i = 0; i < types.size(); i++) {
            out[i] = static_cast<hidlpp::PostProcType>(types[i]);
        }
        _hidl_cb(out);
        return Void();
    }

    Return<void> getCapabilities(hidlpp::PostProcType type, getCapabilities_cb _hidl_cb) override {
        aidlpp::PostProcCapabilities caps;
        mService->getCapabilities(static_cast<aidlpp::PostProcType>(type), &caps);
        _hidl_cb(toHidl(caps));
        return Void();
    }

    Return<sp<hidlpp::IPostProcSession>> createPostProcessor(
            const hidlpp::CreateParams& params,
            const sp<hidlpp::IPostProcServiceCallBacks>& callback) override {
        aidlpp::CreateParams aparams;
        aparams.postProcTypeVal = static_cast<aidlpp::PostProcType>(params.postProcTypeVal);
        aparams.input = toAidl(params.input);
        aparams.output = toAidl(params.output);

        auto cb = ndk::SharedRefBase::make<AidlCallbacks>(callback);
        std::shared_ptr<aidlpp::IPostProcSession> session;
        if (!mService->createPostProcessor(aparams, cb, &session).isOk() || !session) {
            return nullptr;
        }
        return sp<hidlpp::IPostProcSession>(new HidlSession(session, cb));
    }

  private:
    std::shared_ptr<aidlpp::IPostProcService> mService;
};

}  // namespace

// The blobs import IPostProcService::registerAsShimSvc() and getShimSvc(),
// renamed from registerAsService() and getService(). They can't be added as
// members of the generated class, so define them by their mangled names.
status_t registerPostProc(hidlpp::IPostProcService* self, const std::string& name) __asm__(
        "_ZN6vendor3qti8hardware6camera8postproc4V1_016IPostProcService17registerAsShimSvcERKNSt3__"
        "112basic_stringIcNS6_11char_traitsIcEENS6_9allocatorIcEEEE");
sp<hidlpp::IPostProcService> getPostProc(const std::string& name, bool getStub) __asm__(
        "_ZN6vendor3qti8hardware6camera8postproc4V1_016IPostProcService10getShimSvcERKNSt3__"
        "112basic_stringIcNS6_11char_traitsIcEENS6_9allocatorIcEEEEb");

status_t registerPostProc(hidlpp::IPostProcService* self, const std::string&) {
    sp<hidlpp::IPostProcService> service = self;
    {
        std::lock_guard<std::mutex> lock(gLock);
        gLocalPostProc = service;
    }

    auto aidlService = ndk::SharedRefBase::make<AidlService>(service);
    binder_status_t status =
            AServiceManager_addService(aidlService->asBinder().get(), kAidlInstance.c_str());
    if (status != STATUS_OK) {
        LOG(ERROR) << "Failed to register " << kAidlInstance << ": " << status;
        return android::UNKNOWN_ERROR;
    }
    return android::OK;
}

sp<hidlpp::IPostProcService> getPostProc(const std::string&, bool) {
    {
        std::lock_guard<std::mutex> lock(gLock);
        if (gLocalPostProc) return gLocalPostProc;
    }

    auto service = aidlpp::IPostProcService::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(kAidlInstance.c_str())));
    if (!service) {
        LOG(ERROR) << "Failed to get " << kAidlInstance;
        return nullptr;
    }
    return new HidlService(service);
}

namespace vendor::qti::hardware::camera {

namespace aon::V1_0 {

struct IAONService : public IBase {
    status_t registerAsShimSvc(const std::string& name);
};

// Nothing outside the camera provider uses the AON service, so just keep it
// alive instead of declaring it to hwservicemanager.
status_t IAONService::registerAsShimSvc(const std::string&) {
    static sp<IBase> sService = this;
    return android::OK;
}

}  // namespace aon::V1_0

}  // namespace vendor::qti::hardware::camera

// The postproc service looks up the HIDL 4.0 mapper, which is no longer
// declared in VINTF; it is a passthrough HAL anyway, so load it directly.
using android::hardware::graphics::mapper::V4_0::IMapper;

sp<IMapper> getMapper(const std::string& name, bool getStub) __asm__(
        "_ZN7android8hardware8graphics6mapper4V4_07IMapper10getShimSvcERKNSt3__112basic_stringIcNS5_"
        "11char_traitsIcEENS5_9allocatorIcEEEEb");

sp<IMapper> getMapper(const std::string& name, bool) {
    return IMapper::getService(name, true);
}

// NtCam reads vendor tags from the camera.common@1.0 helper globals, which the
// HIDL vendor camera NDK used to set. The AIDL NDK sets the unversioned helper
// globals instead; the classes are the same apart from the namespace, so
// mirror them once the camera service connection is up.
using android::hardware::camera::common::helper::VendorTagDescriptor;
using android::hardware::camera::common::helper::VendorTagDescriptorCache;

struct ACameraManager;
struct ACameraIdList;
using camera_status_t = int32_t;

extern "C" camera_status_t ACameraManager_getCameraIdShim(ACameraManager* manager,
                                                          ACameraIdList** cameraIdList) {
    // Looked up at runtime so the camera provider, which also loads this
    // library, doesn't pull in the vendor camera NDK.
    static auto getCameraIdList =
            reinterpret_cast<camera_status_t (*)(ACameraManager*, ACameraIdList**)>(
                    dlsym(RTLD_DEFAULT, "ACameraManager_getCameraIdList"));
    camera_status_t ret = getCameraIdList(manager, cameraIdList);

    static std::once_flag once;
    std::call_once(once, [] {
        auto getCache = reinterpret_cast<sp<VendorTagDescriptorCache> (*)()>(dlsym(
                RTLD_DEFAULT,
                "_ZN7android8hardware6camera6common6helper24VendorTagDescriptorCache"
                "23getGlobalVendorTagCacheEv"));
        auto setCache = reinterpret_cast<status_t (*)(const sp<VendorTagDescriptorCache>&)>(dlsym(
                RTLD_DEFAULT,
                "_ZN7android8hardware6camera6common4V1_06helper24VendorTagDescriptorCache"
                "25setAsGlobalVendorTagCacheERKNS_2spIS5_EE"));
        auto getDesc = reinterpret_cast<sp<VendorTagDescriptor> (*)()>(dlsym(
                RTLD_DEFAULT,
                "_ZN7android8hardware6camera6common6helper19VendorTagDescriptor"
                "28getGlobalVendorTagDescriptorEv"));
        auto setDesc = reinterpret_cast<status_t (*)(const sp<VendorTagDescriptor>&)>(dlsym(
                RTLD_DEFAULT,
                "_ZN7android8hardware6camera6common4V1_06helper19VendorTagDescriptor"
                "30setAsGlobalVendorTagDescriptorERKNS_2spIS5_EE"));
        if (!getCache || !setCache || !getDesc || !setDesc) {
            LOG(ERROR) << "Vendor tag helper symbols not found";
            return;
        }

        if (sp<VendorTagDescriptorCache> cache = getCache()) setCache(cache);
        if (sp<VendorTagDescriptor> desc = getDesc()) setDesc(desc);
        LOG(INFO) << "Mirrored vendor tag globals";
    });
    return ret;
}
