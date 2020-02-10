/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "dumpstate"

#include "DumpstateDevice.h"

#include <android-base/properties.h>
#include <log/log.h>

#include "DumpstateUtil.h"

#define VENDOR_VERBOSE_LOGGING_ENABLED_PROPERTY "persist.vendor.verbose_logging_enabled"

using android::os::dumpstate::CommandOptions;
using android::os::dumpstate::DumpFileToFd;
using android::os::dumpstate::RunCommandToFd;

namespace android {
namespace hardware {
namespace dumpstate {
namespace V1_1 {
namespace implementation {

// Methods from ::android::hardware::dumpstate::V1_0::IDumpstateDevice follow.
Return<void> DumpstateDevice::dumpstateBoard(const hidl_handle& handle) {
    // Ignore return value, just return an empty status.
    dumpstateBoard_1_1(handle, DumpstateMode::DEFAULT, 30 * 1000 /* timeoutMillis */);
    return Void();
}

// Methods from ::android::hardware::dumpstate::V1_1::IDumpstateDevice follow.
Return<DumpstateStatus> DumpstateDevice::dumpstateBoard_1_1(const hidl_handle& handle,
                                                            const DumpstateMode mode,
                                                            const uint64_t timeoutMillis) {
    // Unused arguments.
    (void) timeoutMillis;

    if (handle == nullptr || handle->numFds < 1) {
        ALOGE("no FDs\n");
        return DumpstateStatus::ILLEGAL_ARGUMENT;
    }

    int fd = handle->data[0];
    if (fd < 0) {
        ALOGE("invalid FD: %d\n", handle->data[0]);
        return DumpstateStatus::ILLEGAL_ARGUMENT;
    }

    if (mode == DumpstateMode::WEAR) {
        // We aren't a Wear device.
        ALOGE("Unsupported mode: %d\n", mode);
        return DumpstateStatus::UNSUPPORTED_MODE;
    } else if (mode < DumpstateMode::FULL || mode > DumpstateMode::DEFAULT) {
        ALOGE("Invalid mode: %d\n", mode);
        return DumpstateStatus::ILLEGAL_ARGUMENT;
    }

    DumpFileToFd(fd, "TZ ramoops annotation", "/sys/fs/pstore/annotate-ramoops");
    DumpFileToFd(fd, "Recent panic log", "/sys/fs/pstore/dmesg-ramoops-0");
    DumpFileToFd(fd, "cpuinfo", "/proc/cpuinfo");
    DumpFileToFd(fd, "Interrupts", "/proc/interrupts");
    DumpFileToFd(fd, "Power Management Stats", "/proc/msm_pm_stats");
    DumpFileToFd(fd, "RPM Stats", "/d/rpm_stats");
    DumpFileToFd(fd, "SMB135x Config Regs", "/d/smb135x/config_registers");
    DumpFileToFd(fd, "SMB135x IRQ Count", "/d/smb135x/irq_count");
    DumpFileToFd(fd, "SMB135x Status Regs", "/d/smb135x/status_registers");
    DumpFileToFd(fd, "wlan", "/sys/module/bcmdhd/parameters/info_string");
    DumpFileToFd(fd, "Battery Statistics", "/sys/class/power_supply/battery/uevent");
    DumpFileToFd(fd, "PCIe IPC Logging", "/d/ipc_logging/pcie0/log");
    DumpFileToFd(fd, "HSIC IPC Control Logging", "/d/xhci_msm_hsic_dbg/show_ctrl_events");
    DumpFileToFd(fd, "HSIC IPC Data Logging", "/d/xhci_msm_hsic_dbg/show_data_events");
    DumpFileToFd(fd, "ION kmalloc heap", "/d/ion/heaps/kmalloc");
    DumpFileToFd(fd, "ION multimedia heap", "/d/ion/heaps/mm");
    DumpFileToFd(fd, "ION peripheral-image-loader heap", "/d/ion/heaps/pil_1");
    DumpFileToFd(fd, "ION secure-comm heap", "/d/ion/heaps/qsecom");
    DumpFileToFd(fd, "ION system heap", "/d/ion/heaps/system");
    DumpFileToFd(fd, "HSIC control events", "/d/xhci_msm_hsic_dbg/show_ctrl_events");
    DumpFileToFd(fd, "HSIC data events", "/d/xhci_msm_hsic_dbg/show_data_events");
    DumpFileToFd(fd, "USB PM events", "/d/usb_pm_hsic_dbg/show_usb_pm_events");
    DumpFileToFd(fd, "MDSS registers", "/d/mdp/reg_dump");
    RunCommandToFd(fd, "Subsystem Tombstone list", {"ls", "-l", "/data/tombstones/ramdump"}, CommandOptions::AS_ROOT);
    RunCommandToFd(fd, "ION CLIENTS", {"/system/bin/sh", "-c", "for f in $(ls /d/ion/clients/*); do echo $f; cat $f; done"}, CommandOptions::AS_ROOT);
    RunCommandToFd(fd, "ION HEAPS",   {"/system/bin/sh", "-c", "for f in $(ls /d/ion/heaps/*);   do echo $f; cat $f; done"}, CommandOptions::AS_ROOT);

    return DumpstateStatus::OK;
}

Return<void> DumpstateDevice::setVerboseLoggingEnabled(const bool enable) {
    android::base::SetProperty(VENDOR_VERBOSE_LOGGING_ENABLED_PROPERTY, enable ? "true" : "false");
    return Void();
}

Return<bool> DumpstateDevice::getVerboseLoggingEnabled() {
    return android::base::GetBoolProperty(VENDOR_VERBOSE_LOGGING_ENABLED_PROPERTY, false);
}

}  // namespace implementation
}  // namespace V1_1
}  // namespace dumpstate
}  // namespace hardware
}  // namespace android
