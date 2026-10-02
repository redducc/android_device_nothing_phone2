/*
 * SPDX-FileCopyrightText: Paranoid Android
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android/hidl/base/1.0/IBase.h>
#include <utils/Errors.h>

#include <string>

using android::sp;
using android::status_t;
using android::hidl::base::V1_0::IBase;

namespace vendor::qti::hardware::camera::aon::V1_0 {

struct IAONService : public IBase {
    status_t registerAsShimSvc(const std::string& name);
};

// CamX registers the AON service but nothing outside the camera provider uses
// it, so just keep it alive instead of declaring it to hwservicemanager.
status_t IAONService::registerAsShimSvc(const std::string&) {
    static sp<IBase> sService = this;
    return android::OK;
}

}  // namespace vendor::qti::hardware::camera::aon::V1_0
