#ifndef HIDL_GENERATED_VENDOR_QTI_HARDWARE_CAMERA_POSTPROC_V1_0_TYPES_H
#define HIDL_GENERATED_VENDOR_QTI_HARDWARE_CAMERA_POSTPROC_V1_0_TYPES_H

#include <hidl/HidlSupport.h>
#include <hidl/MQDescriptor.h>
#include <utils/NativeHandle.h>
#include <utils/misc.h>

namespace vendor {
namespace qti {
namespace hardware {
namespace camera {
namespace postproc {
namespace V1_0 {

// Forward declaration for forward reference support:
enum class Error : int32_t;
enum class PostProcType : int32_t;
struct Resolution;
struct JpegCapabilities;
struct PostProcCapabilities;
struct BufferParams;
struct CreateParams;
struct HandleParams;
struct ProcessRequestParams;
struct JpegResult;
struct PostProcResult;

/**
 * Camera metadata is provided as byte array.
 */
typedef ::android::hardware::hidl_vec<uint8_t> CameraMetadata;

/**
 * This is generic Error enum
 */
enum class Error : int32_t {
    /**
     * Success
     */
    NONE = 0,
    /**
     * StreamId out of range
     */
    BAD_STREAMID = 1,
    /**
     * Maximum Sessions are in Queue
     */
    MAX_SESSIONS = 2,
    /**
     * Handle pointer is invalid
     */
    INVALID_HANDLE = 3,
    /**
     * postproc Session Create Failed
     */
    SESSION_NOT_INIT = 4,
    /**
     * Malloc failed
     */
    MALLOC_FAIL = 5,
    /**
     * Post Processor failed
     */
    POSTPROC_FAIL = 6,
    /**
     * Device is in bad state
     */
    DEVICE_BAD_STATE = 7,
    /**
     * CB Pointer is invalid
     */
    INVALID_CALLBACK_PTR = 8,
    /**
     * PostProc Aborted
     */
    ABORT = 9,
    /**
     * Unsupported Resolution
     */
    UNSUPPORTED_RESOLUTION = 10,
};

/**
 * PostProcessor types supported by the service
 */
enum class PostProcType : int32_t {
    /**
     * YUV2Jpeg Conversion
     */
    JPEG = 0,
};

/*
 * This struct contains WxH parameters
 */
struct Resolution final {
    /**
     * Width
     */
    uint32_t width __attribute__ ((aligned(4)));
    /**
     * Height
     */
    uint32_t height __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::Resolution, width) == 0, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::Resolution, height) == 4, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::Resolution) == 8, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::Resolution) == 4, "wrong alignment");

/**
 * This struct contains JPEG postproc capabilities.
 * Dynamic update of resolution is supported.
 */
struct JpegCapabilities final {
    /**
     * Max number of streams supported
     */
    uint32_t maxStreamsSupported __attribute__ ((aligned(4)));
    /**
     * Max Resolution supported
     */
    ::vendor::qti::hardware::camera::postproc::V1_0::Resolution maxResoultion __attribute__ ((aligned(4)));
    /**
     * Min Resolution supported
     */
    ::vendor::qti::hardware::camera::postproc::V1_0::Resolution minResolution __attribute__ ((aligned(4)));
    /**
     * Gralloc Formats supported
     */
    ::android::hardware::hidl_vec<uint32_t> formats __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities, maxStreamsSupported) == 0, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities, maxResoultion) == 4, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities, minResolution) == 12, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities, formats) == 24, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities) == 40, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities) == 8, "wrong alignment");

/**
 * This structure contains generic PostProc capabilities info.
 * This will be updated based on different postproc features supported
 */
struct PostProcCapabilities final {
    /**
     * JPEG stream capabilities
     */
    ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities jpegStream __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities, jpegStream) == 0, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities) == 40, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities) == 8, "wrong alignment");

/**
 * This struct contains Input and output Buffer generic properties
 */
struct BufferParams final {
    /**
     * Gralloc Format for handle
     */
    uint32_t format __attribute__ ((aligned(4)));
    /**
     * Width
     */
    uint32_t width __attribute__ ((aligned(4)));
    /**
     * Height
     */
    uint32_t height __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::BufferParams, format) == 0, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::BufferParams, width) == 4, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::BufferParams, height) == 8, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::BufferParams) == 12, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::BufferParams) == 4, "wrong alignment");

/**
 * This struct contains parameters for postproc initialization
 */
struct CreateParams final {
    /**
     * Postproc type value
     */
    ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType postProcTypeVal __attribute__ ((aligned(4)));
    /**
     * Parameters for input
     */
    ::android::hardware::hidl_vec<::vendor::qti::hardware::camera::postproc::V1_0::BufferParams> input __attribute__ ((aligned(8)));
    /**
     * Parameters for output
     */
    ::android::hardware::hidl_vec<::vendor::qti::hardware::camera::postproc::V1_0::BufferParams> output __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::CreateParams, postProcTypeVal) == 0, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::CreateParams, input) == 8, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::CreateParams, output) == 24, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::CreateParams) == 40, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::CreateParams) == 8, "wrong alignment");

/**
 * Client can dynamically choose to update resolution for JPEG encode.
 * If Client choose to use same resolution, then same parameters as CreateParams can be used
 */
struct HandleParams final {
    /**
     * Gralloc Format for handle
     */
    uint32_t format __attribute__ ((aligned(4)));
    /**
     * Width
     */
    uint32_t width __attribute__ ((aligned(4)));
    /**
     * Height
     */
    uint32_t height __attribute__ ((aligned(4)));
    /**
     * handle pointer
     */
    ::android::hardware::hidl_handle bufHandle __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::HandleParams, format) == 0, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::HandleParams, width) == 4, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::HandleParams, height) == 8, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::HandleParams, bufHandle) == 16, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::HandleParams) == 32, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::HandleParams) == 8, "wrong alignment");

/**
 * This structure contains parameters given during postproc process request
 */
struct ProcessRequestParams final {
    /**
     * Array of input handles
     */
    ::android::hardware::hidl_vec<::vendor::qti::hardware::camera::postproc::V1_0::HandleParams> input __attribute__ ((aligned(8)));
    /**
     * Arry of output handles.
     */
    ::android::hardware::hidl_vec<::vendor::qti::hardware::camera::postproc::V1_0::HandleParams> output __attribute__ ((aligned(8)));
    /**
     * Stream id, This indicates which index parameters provided during Init to use for encoding.
     */
    uint32_t streamId __attribute__ ((aligned(4)));
    /**
     * Metadata related to Camera
     */
    ::android::hardware::hidl_vec<uint8_t> metadata __attribute__ ((aligned(8)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams, input) == 0, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams, output) == 16, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams, streamId) == 32, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams, metadata) == 40, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams) == 56, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams) == 8, "wrong alignment");

/**
 * JPEG PostProc Result structure
 */
struct JpegResult final {
    /**
     * Encoded frame size
     */
    uint32_t frameSize __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::JpegResult, frameSize) == 0, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::JpegResult) == 4, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::JpegResult) == 4, "wrong alignment");

/**
 * This is generic result structure.
 * Based on PostProcType, corresponding result structre will be used.
 */
struct PostProcResult final {
    /**
     * requestId given to client as part of process API
     */
    uint32_t requestId __attribute__ ((aligned(4)));
    /**
     * stream
     */
    uint32_t streamId __attribute__ ((aligned(4)));
    /**
     * Postproc Type Value
     */
    ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType postProcTypeVal __attribute__ ((aligned(4)));
    /**
     * JPEG Result structure
     */
    ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult jpegResult __attribute__ ((aligned(4)));
};

static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult, requestId) == 0, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult, streamId) == 4, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult, postProcTypeVal) == 8, "wrong offset");
static_assert(offsetof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult, jpegResult) == 12, "wrong offset");
static_assert(sizeof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult) == 16, "wrong size");
static_assert(__alignof(::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult) == 4, "wrong alignment");

//
// type declarations for package
//

template<typename>
static inline std::string toString(int32_t o);
static inline std::string toString(::vendor::qti::hardware::camera::postproc::V1_0::Error o);
static inline void PrintTo(::vendor::qti::hardware::camera::postproc::V1_0::Error o, ::std::ostream* os);
constexpr int32_t operator|(const ::vendor::qti::hardware::camera::postproc::V1_0::Error lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Error rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const int32_t lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Error rhs) {
    return static_cast<int32_t>(lhs | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const ::vendor::qti::hardware::camera::postproc::V1_0::Error lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | rhs);
}
constexpr int32_t operator&(const ::vendor::qti::hardware::camera::postproc::V1_0::Error lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Error rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const int32_t lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Error rhs) {
    return static_cast<int32_t>(lhs & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const ::vendor::qti::hardware::camera::postproc::V1_0::Error lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & rhs);
}
constexpr int32_t &operator|=(int32_t& v, const ::vendor::qti::hardware::camera::postproc::V1_0::Error e) {
    v |= static_cast<int32_t>(e);
    return v;
}
constexpr int32_t &operator&=(int32_t& v, const ::vendor::qti::hardware::camera::postproc::V1_0::Error e) {
    v &= static_cast<int32_t>(e);
    return v;
}

template<typename>
static inline std::string toString(int32_t o);
static inline std::string toString(::vendor::qti::hardware::camera::postproc::V1_0::PostProcType o);
static inline void PrintTo(::vendor::qti::hardware::camera::postproc::V1_0::PostProcType o, ::std::ostream* os);
constexpr int32_t operator|(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const int32_t lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType rhs) {
    return static_cast<int32_t>(lhs | static_cast<int32_t>(rhs));
}
constexpr int32_t operator|(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) | rhs);
}
constexpr int32_t operator&(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const int32_t lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType rhs) {
    return static_cast<int32_t>(lhs & static_cast<int32_t>(rhs));
}
constexpr int32_t operator&(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType lhs, const int32_t rhs) {
    return static_cast<int32_t>(static_cast<int32_t>(lhs) & rhs);
}
constexpr int32_t &operator|=(int32_t& v, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType e) {
    v |= static_cast<int32_t>(e);
    return v;
}
constexpr int32_t &operator&=(int32_t& v, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType e) {
    v &= static_cast<int32_t>(e);
    return v;
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& o, ::std::ostream*);
static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& rhs);
static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& rhs);

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& o, ::std::ostream*);
static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& rhs);
static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& rhs);

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& o, ::std::ostream*);
static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& rhs);
static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& rhs);

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& o, ::std::ostream*);
static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& rhs);
static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& rhs);

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& o, ::std::ostream*);
static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& rhs);
static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& rhs);

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::HandleParams& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::HandleParams& o, ::std::ostream*);
// operator== and operator!= are not generated for HandleParams

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams& o, ::std::ostream*);
// operator== and operator!= are not generated for ProcessRequestParams

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& o, ::std::ostream*);
static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& rhs);
static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& rhs);

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& o);
static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& o, ::std::ostream*);
static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& rhs);
static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& rhs);

//
// type header definitions for package
//

template<>
inline std::string toString<::vendor::qti::hardware::camera::postproc::V1_0::Error>(int32_t o) {
    using ::android::hardware::details::toHexString;
    std::string os;
    ::android::hardware::hidl_bitfield<::vendor::qti::hardware::camera::postproc::V1_0::Error> flipped = 0;
    bool first = true;
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::NONE) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::NONE)) {
        os += (first ? "" : " | ");
        os += "NONE";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::NONE;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::BAD_STREAMID) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::BAD_STREAMID)) {
        os += (first ? "" : " | ");
        os += "BAD_STREAMID";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::BAD_STREAMID;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::MAX_SESSIONS) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::MAX_SESSIONS)) {
        os += (first ? "" : " | ");
        os += "MAX_SESSIONS";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::MAX_SESSIONS;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_HANDLE) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_HANDLE)) {
        os += (first ? "" : " | ");
        os += "INVALID_HANDLE";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_HANDLE;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::SESSION_NOT_INIT) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::SESSION_NOT_INIT)) {
        os += (first ? "" : " | ");
        os += "SESSION_NOT_INIT";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::SESSION_NOT_INIT;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::MALLOC_FAIL) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::MALLOC_FAIL)) {
        os += (first ? "" : " | ");
        os += "MALLOC_FAIL";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::MALLOC_FAIL;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::POSTPROC_FAIL) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::POSTPROC_FAIL)) {
        os += (first ? "" : " | ");
        os += "POSTPROC_FAIL";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::POSTPROC_FAIL;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::DEVICE_BAD_STATE) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::DEVICE_BAD_STATE)) {
        os += (first ? "" : " | ");
        os += "DEVICE_BAD_STATE";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::DEVICE_BAD_STATE;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_CALLBACK_PTR) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_CALLBACK_PTR)) {
        os += (first ? "" : " | ");
        os += "INVALID_CALLBACK_PTR";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_CALLBACK_PTR;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::ABORT) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::ABORT)) {
        os += (first ? "" : " | ");
        os += "ABORT";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::ABORT;
    }
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::Error::UNSUPPORTED_RESOLUTION) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::Error::UNSUPPORTED_RESOLUTION)) {
        os += (first ? "" : " | ");
        os += "UNSUPPORTED_RESOLUTION";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::Error::UNSUPPORTED_RESOLUTION;
    }
    if (o != flipped) {
        os += (first ? "" : " | ");
        os += toHexString(o & (~flipped));
    }os += " (";
    os += toHexString(o);
    os += ")";
    return os;
}

static inline std::string toString(::vendor::qti::hardware::camera::postproc::V1_0::Error o) {
    using ::android::hardware::details::toHexString;
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::NONE) {
        return "NONE";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::BAD_STREAMID) {
        return "BAD_STREAMID";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::MAX_SESSIONS) {
        return "MAX_SESSIONS";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_HANDLE) {
        return "INVALID_HANDLE";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::SESSION_NOT_INIT) {
        return "SESSION_NOT_INIT";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::MALLOC_FAIL) {
        return "MALLOC_FAIL";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::POSTPROC_FAIL) {
        return "POSTPROC_FAIL";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::DEVICE_BAD_STATE) {
        return "DEVICE_BAD_STATE";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_CALLBACK_PTR) {
        return "INVALID_CALLBACK_PTR";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::ABORT) {
        return "ABORT";
    }
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::Error::UNSUPPORTED_RESOLUTION) {
        return "UNSUPPORTED_RESOLUTION";
    }
    std::string os;
    os += toHexString(static_cast<int32_t>(o));
    return os;
}

static inline void PrintTo(::vendor::qti::hardware::camera::postproc::V1_0::Error o, ::std::ostream* os) {
    *os << toString(o);
}

template<>
inline std::string toString<::vendor::qti::hardware::camera::postproc::V1_0::PostProcType>(int32_t o) {
    using ::android::hardware::details::toHexString;
    std::string os;
    ::android::hardware::hidl_bitfield<::vendor::qti::hardware::camera::postproc::V1_0::PostProcType> flipped = 0;
    bool first = true;
    if ((o & ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType::JPEG) == static_cast<int32_t>(::vendor::qti::hardware::camera::postproc::V1_0::PostProcType::JPEG)) {
        os += (first ? "" : " | ");
        os += "JPEG";
        first = false;
        flipped |= ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType::JPEG;
    }
    if (o != flipped) {
        os += (first ? "" : " | ");
        os += toHexString(o & (~flipped));
    }os += " (";
    os += toHexString(o);
    os += ")";
    return os;
}

static inline std::string toString(::vendor::qti::hardware::camera::postproc::V1_0::PostProcType o) {
    using ::android::hardware::details::toHexString;
    if (o == ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType::JPEG) {
        return "JPEG";
    }
    std::string os;
    os += toHexString(static_cast<int32_t>(o));
    return os;
}

static inline void PrintTo(::vendor::qti::hardware::camera::postproc::V1_0::PostProcType o, ::std::ostream* os) {
    *os << toString(o);
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".width = ";
    os += ::android::hardware::toString(o.width);
    os += ", .height = ";
    os += ::android::hardware::toString(o.height);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& rhs) {
    if (lhs.width != rhs.width) {
        return false;
    }
    if (lhs.height != rhs.height) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::Resolution& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".maxStreamsSupported = ";
    os += ::android::hardware::toString(o.maxStreamsSupported);
    os += ", .maxResoultion = ";
    os += ::vendor::qti::hardware::camera::postproc::V1_0::toString(o.maxResoultion);
    os += ", .minResolution = ";
    os += ::vendor::qti::hardware::camera::postproc::V1_0::toString(o.minResolution);
    os += ", .formats = ";
    os += ::android::hardware::toString(o.formats);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& rhs) {
    if (lhs.maxStreamsSupported != rhs.maxStreamsSupported) {
        return false;
    }
    if (lhs.maxResoultion != rhs.maxResoultion) {
        return false;
    }
    if (lhs.minResolution != rhs.minResolution) {
        return false;
    }
    if (lhs.formats != rhs.formats) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegCapabilities& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".jpegStream = ";
    os += ::vendor::qti::hardware::camera::postproc::V1_0::toString(o.jpegStream);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& rhs) {
    if (lhs.jpegStream != rhs.jpegStream) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcCapabilities& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".format = ";
    os += ::android::hardware::toString(o.format);
    os += ", .width = ";
    os += ::android::hardware::toString(o.width);
    os += ", .height = ";
    os += ::android::hardware::toString(o.height);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& rhs) {
    if (lhs.format != rhs.format) {
        return false;
    }
    if (lhs.width != rhs.width) {
        return false;
    }
    if (lhs.height != rhs.height) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::BufferParams& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".postProcTypeVal = ";
    os += ::vendor::qti::hardware::camera::postproc::V1_0::toString(o.postProcTypeVal);
    os += ", .input = ";
    os += ::android::hardware::toString(o.input);
    os += ", .output = ";
    os += ::android::hardware::toString(o.output);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& rhs) {
    if (lhs.postProcTypeVal != rhs.postProcTypeVal) {
        return false;
    }
    if (lhs.input != rhs.input) {
        return false;
    }
    if (lhs.output != rhs.output) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::CreateParams& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::HandleParams& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".format = ";
    os += ::android::hardware::toString(o.format);
    os += ", .width = ";
    os += ::android::hardware::toString(o.width);
    os += ", .height = ";
    os += ::android::hardware::toString(o.height);
    os += ", .bufHandle = ";
    os += ::android::hardware::toString(o.bufHandle);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::HandleParams& o, ::std::ostream* os) {
    *os << toString(o);
}

// operator== and operator!= are not generated for HandleParams

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".input = ";
    os += ::android::hardware::toString(o.input);
    os += ", .output = ";
    os += ::android::hardware::toString(o.output);
    os += ", .streamId = ";
    os += ::android::hardware::toString(o.streamId);
    os += ", .metadata = ";
    os += ::android::hardware::toString(o.metadata);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::ProcessRequestParams& o, ::std::ostream* os) {
    *os << toString(o);
}

// operator== and operator!= are not generated for ProcessRequestParams

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".frameSize = ";
    os += ::android::hardware::toString(o.frameSize);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& rhs) {
    if (lhs.frameSize != rhs.frameSize) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::JpegResult& rhs){
    return !(lhs == rhs);
}

static inline std::string toString(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& o) {
    using ::android::hardware::toString;
    std::string os;
    os += "{";
    os += ".requestId = ";
    os += ::android::hardware::toString(o.requestId);
    os += ", .streamId = ";
    os += ::android::hardware::toString(o.streamId);
    os += ", .postProcTypeVal = ";
    os += ::vendor::qti::hardware::camera::postproc::V1_0::toString(o.postProcTypeVal);
    os += ", .jpegResult = ";
    os += ::vendor::qti::hardware::camera::postproc::V1_0::toString(o.jpegResult);
    os += "}"; return os;
}

static inline void PrintTo(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& o, ::std::ostream* os) {
    *os << toString(o);
}

static inline bool operator==(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& rhs) {
    if (lhs.requestId != rhs.requestId) {
        return false;
    }
    if (lhs.streamId != rhs.streamId) {
        return false;
    }
    if (lhs.postProcTypeVal != rhs.postProcTypeVal) {
        return false;
    }
    if (lhs.jpegResult != rhs.jpegResult) {
        return false;
    }
    return true;
}

static inline bool operator!=(const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& lhs, const ::vendor::qti::hardware::camera::postproc::V1_0::PostProcResult& rhs){
    return !(lhs == rhs);
}


}  // namespace V1_0
}  // namespace postproc
}  // namespace camera
}  // namespace hardware
}  // namespace qti
}  // namespace vendor

//
// global type declarations for package
//

namespace android {
namespace hardware {
namespace details {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template<> inline constexpr std::array<::vendor::qti::hardware::camera::postproc::V1_0::Error, 11> hidl_enum_values<::vendor::qti::hardware::camera::postproc::V1_0::Error> = {
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::NONE,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::BAD_STREAMID,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::MAX_SESSIONS,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_HANDLE,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::SESSION_NOT_INIT,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::MALLOC_FAIL,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::POSTPROC_FAIL,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::DEVICE_BAD_STATE,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::INVALID_CALLBACK_PTR,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::ABORT,
    ::vendor::qti::hardware::camera::postproc::V1_0::Error::UNSUPPORTED_RESOLUTION,
};
#pragma clang diagnostic pop
}  // namespace details
}  // namespace hardware
}  // namespace android

namespace android {
namespace hardware {
namespace details {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
template<> inline constexpr std::array<::vendor::qti::hardware::camera::postproc::V1_0::PostProcType, 1> hidl_enum_values<::vendor::qti::hardware::camera::postproc::V1_0::PostProcType> = {
    ::vendor::qti::hardware::camera::postproc::V1_0::PostProcType::JPEG,
};
#pragma clang diagnostic pop
}  // namespace details
}  // namespace hardware
}  // namespace android


#endif  // HIDL_GENERATED_VENDOR_QTI_HARDWARE_CAMERA_POSTPROC_V1_0_TYPES_H
