/*
 * SPDX-FileCopyrightText: 2024-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/android/hardware/power/BnPower.h>
#include <aidl/vendor/oplus/hardware/touch/IOplusTouch.h>

#include <android-base/logging.h>
#include <android/binder_manager.h>

#include <OplusTouchConstants.h>

using aidl::android::hardware::power::Mode;
using aidl::vendor::oplus::hardware::touch::IOplusTouch;

#ifdef LIBPERFMGR_EXT
namespace aidl::google::hardware::power::impl::pixel {
#else
namespace aidl::android::hardware::power::impl {
#endif

bool isDeviceSpecificModeSupported(Mode type, bool* _aidl_return) {
    switch (type) {
        case Mode::DOUBLE_TAP_TO_WAKE:
            *_aidl_return = true;
            return true;
        default:
            return false;
    }
}

bool setDeviceSpecificMode(Mode type, bool enabled) {
    switch (type) {
        case Mode::DOUBLE_TAP_TO_WAKE: {
            const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
            std::shared_ptr<IOplusTouch> oplusTouch = IOplusTouch::fromBinder(
                    ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
            LOG(INFO) << "Power mode: " << toString(type) << " isDoubleTapEnabled: " << enabled;

            if (!oplusTouch) {
                LOG(ERROR) << "Failed to get OPLUS touch HAL";
                return false;
            }

            int32_t indepSupported = 0;
            if (!oplusTouch->isTouchNodeSupport(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                                OplusTouchConstants::DOUBLE_TAP_INDEP_NODE,
                                                &indepSupported)
                         .isOk()) {
                LOG(ERROR) << "Failed to query OPLUS double-tap independent node";
                return false;
            }

            int32_t simpleSupported = 0;
            if (!oplusTouch->isTouchNodeSupport(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                                OplusTouchConstants::DOUBLE_TAP_ENABLE_NODE,
                                                &simpleSupported)
                         .isOk()) {
                LOG(ERROR) << "Failed to query OPLUS double-tap node";
                return false;
            }

            if (indepSupported > 0) {
                std::string tmp;
                oplusTouch->touchReadNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                              OplusTouchConstants::DOUBLE_TAP_INDEP_NODE, &tmp);
                if (tmp.empty()) {
                    LOG(ERROR) << "Failed to read OPLUS double-tap node";
                    return false;
                }

                int contents = std::stoi(tmp, nullptr, 16);
                if (enabled) {
                    contents |= OplusTouchConstants::DOUBLE_TAP_GESTURE;
                } else {
                    contents &= ~OplusTouchConstants::DOUBLE_TAP_GESTURE;
                }

                oplusTouch->touchWriteNodeFileOneWay(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                                     OplusTouchConstants::DOUBLE_TAP_ENABLE_NODE,
                                                     "1");
                oplusTouch->touchWriteNodeFileOneWay(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                                     OplusTouchConstants::DOUBLE_TAP_INDEP_NODE,
                                                     std::to_string(contents));
                return true;
            }

            if (simpleSupported > 0) {
                oplusTouch->touchWriteNodeFileOneWay(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                                     OplusTouchConstants::DOUBLE_TAP_ENABLE_NODE,
                                                     enabled ? "1" : "0");
                return true;
            }

            LOG(ERROR) << "OPLUS touch HAL has no usable double-tap node";
            return false;
        }
        default:
            return false;
    }
}

}  // namespace
