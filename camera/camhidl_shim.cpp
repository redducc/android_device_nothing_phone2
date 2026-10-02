/*
 * SPDX-FileCopyrightText: Paranoid Android
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android/hidl/base/1.0/IBase.h>
#include <utils/Errors.h>

#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <string>

using android::sp;
using android::status_t;
using android::hidl::base::V1_0::IBase;

// CamX registers these services and the NtCam nodes look them up from inside
// the camera provider itself, so keep them in-process instead of declaring
// them to hwservicemanager. In-process HIDL lookups return the raw object
// anyway, so callers see no difference.
namespace {

std::mutex gLock;
std::condition_variable gCond;
std::map<std::string, sp<IBase>> gServices;

status_t registerService(const std::string& key, IBase* service) {
    std::lock_guard<std::mutex> lock(gLock);
    gServices[key] = service;
    gCond.notify_all();
    return android::OK;
}

sp<IBase> getService(const std::string& key) {
    std::unique_lock<std::mutex> lock(gLock);
    gCond.wait_for(lock, std::chrono::seconds(1), [&] { return gServices.count(key) != 0; });
    auto it = gServices.find(key);
    return it != gServices.end() ? it->second : nullptr;
}

}  // namespace

namespace vendor::qti::hardware::camera {

namespace postproc::V1_0 {
struct IPostProcService : public IBase {
    status_t registerAsShimSvc(const std::string& name);
    static sp<IBase> getShimSvc(const std::string& name, bool getStub);
};

status_t IPostProcService::registerAsShimSvc(const std::string& name) {
    return registerService("postproc/" + name, this);
}

sp<IBase> IPostProcService::getShimSvc(const std::string& name, bool) {
    return getService("postproc/" + name);
}
}  // namespace postproc::V1_0

namespace aon::V1_0 {
struct IAONService : public IBase {
    status_t registerAsShimSvc(const std::string& name);
};

status_t IAONService::registerAsShimSvc(const std::string& name) {
    return registerService("aon/" + name, this);
}
}  // namespace aon::V1_0

}  // namespace vendor::qti::hardware::camera
