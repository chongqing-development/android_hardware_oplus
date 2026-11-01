/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor-oplus-hardware-touch-V2-service"

#include "OplusTouch.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <cstdlib>
#include <string>

using aidl::vendor::oplus::hardware::touch::OplusTouch;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<OplusTouch> service = ndk::SharedRefBase::make<OplusTouch>();
    const std::string instance = std::string(OplusTouch::descriptor) + "/default";
    const binder_status_t status =
            AServiceManager_addService(service->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instance << " " << status;

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;
}
