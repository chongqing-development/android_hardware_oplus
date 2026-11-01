/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/oplus/hardware/touch/BnOplusTouch.h>
#include <aidl/vendor/oplus/hardware/touch/OplusTouchInfo.h>

#include <string>

namespace aidl::vendor::oplus::hardware::touch {

class OplusTouch : public BnOplusTouch {
  public:
    ndk::ScopedAStatus initialize(int32_t* _aidl_return) override;
    ndk::ScopedAStatus isTouchNodeSupport(int32_t deviceId, int32_t nodeFlag,
                                          int32_t* _aidl_return) override;
    ndk::ScopedAStatus touchReadNodeFile(int32_t deviceId, int32_t nodeFlag,
                                         std::string* _aidl_return) override;
    ndk::ScopedAStatus touchWriteNodeFile(int32_t deviceId, int32_t nodeFlag,
                                          const std::string& info,
                                          int32_t* _aidl_return) override;
    ndk::ScopedAStatus touchWriteBtInfo(int32_t deviceId, int32_t nodeFlag,
                                        const std::string& info,
                                        int32_t* _aidl_return) override;
    ndk::ScopedAStatus touchWriteNodeFileOneWay(int32_t deviceId, int32_t nodeFlag,
                                                const std::string& info) override;
    ndk::ScopedAStatus touchNotifyClient(int32_t clientFlag, const OplusTouchInfo& info,
                                          int32_t* _aidl_return) override;

  private:
    static std::string getNodePath(int32_t deviceId, int32_t nodeFlag);
};

}  // namespace aidl::vendor::oplus::hardware::touch
