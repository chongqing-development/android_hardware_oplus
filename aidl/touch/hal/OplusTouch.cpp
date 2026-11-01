/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OplusTouch.h"

#include <android-base/file.h>
#include <android-base/logging.h>

#include <unistd.h>

#include <map>

namespace aidl::vendor::oplus::hardware::touch {

namespace {

constexpr int32_t kDoubleTapEnableNode = 1;
constexpr int32_t kDoubleTapIndepNode = 21;
constexpr int32_t kGameSwitchEnableNode = 26;
constexpr int32_t kGloveModeEnableNode = 192;

const std::map<int32_t, const char*> kNodeNames = {
        {kDoubleTapEnableNode, "double_tap_enable"},
        {kDoubleTapIndepNode, "double_tap_enable_indep"},
        {kGameSwitchEnableNode, "game_switch_enable"},
        {kGloveModeEnableNode, "glove_mode_enable"},
};

}  // namespace

std::string OplusTouch::getNodePath(int32_t deviceId, int32_t nodeFlag) {
    const auto node = kNodeNames.find(nodeFlag);
    if (node == kNodeNames.end()) {
        return {};
    }

    if (deviceId == 0) {
        return std::string("/proc/touchpanel/") + node->second;
    }
    if (deviceId == 1) {
        return std::string("/proc/touchpanel1/") + node->second;
    }
    return {};
}

ndk::ScopedAStatus OplusTouch::initialize(int32_t* _aidl_return) {
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusTouch::isTouchNodeSupport(int32_t deviceId, int32_t nodeFlag,
                                                  int32_t* _aidl_return) {
    const std::string path = getNodePath(deviceId, nodeFlag);
    *_aidl_return = (!path.empty() && access(path.c_str(), F_OK) == 0) ? 1 : -1;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusTouch::touchReadNodeFile(int32_t deviceId, int32_t nodeFlag,
                                                 std::string* _aidl_return) {
    const std::string path = getNodePath(deviceId, nodeFlag);
    _aidl_return->clear();

    if (path.empty() || !android::base::ReadFileToString(path, _aidl_return)) {
        LOG(ERROR) << "Failed to read touch node, device=" << deviceId
                   << " flag=" << nodeFlag << " path=" << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusTouch::touchWriteNodeFile(int32_t deviceId, int32_t nodeFlag,
                                                  const std::string& info,
                                                  int32_t* _aidl_return) {
    const std::string path = getNodePath(deviceId, nodeFlag);
    *_aidl_return = (!path.empty() && android::base::WriteStringToFile(info, path, true)) ? 0 : -1;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusTouch::touchWriteBtInfo(int32_t deviceId, int32_t nodeFlag,
                                                const std::string& info,
                                                int32_t* _aidl_return) {
    (void)deviceId;
    (void)nodeFlag;
    (void)info;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusTouch::touchWriteNodeFileOneWay(int32_t deviceId, int32_t nodeFlag,
                                                        const std::string& info) {
    int32_t result = 0;
    touchWriteNodeFile(deviceId, nodeFlag, info, &result);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusTouch::touchNotifyClient(int32_t clientFlag,
                                                 const OplusTouchInfo& info,
                                                 int32_t* _aidl_return) {
    (void)clientFlag;
    (void)info;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::vendor::oplus::hardware::touch
