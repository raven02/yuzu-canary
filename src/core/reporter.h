// Copyright 2019 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "common/common_types.h"

union ResultCode;

namespace Kernel {
class HLERequestContext;
} // namespace Kernel

namespace Service::FileSystem {
enum class LogMode : u32;
}

namespace Core {

class System;

class Reporter {
public:
    explicit Reporter(System& system);
    ~Reporter();

    void SaveCrashReport(u64 title_id, ResultCode result, u64 set_flags, u64 entry_point, u64 sp,
                         u64 pc, u64 pstate, u64 afsr0, u64 afsr1, u64 esr, u64 far,
                         const std::array<u64, 31>& registers, const std::array<u64, 32>& backtrace,
                         u32 backtrace_size, const std::string& arch, u32 unk10) const;

    void SaveSvcBreakReport(u32 type, bool signal_debugger, u64 info1, u64 info2,
                            std::optional<std::vector<u8>> resolved_buffer = {}) const;

    void SaveUnimplementedFunctionReport(Kernel::HLERequestContext& ctx, u32 command_id,
                                         const std::string& name,
                                         const std::string& service_name) const;

    void SaveUnimplementedAppletReport(u32 applet_id, u32 common_args_version, u32 library_version,
                                       u32 theme_color, bool startup_sound, u64 system_tick,
                                       std::vector<std::vector<u8>> normal_channel,
                                       std::vector<std::vector<u8>> interactive_channel) const;

    enum class PlayReportType {
        Old,
        New,
        System,
    };

    void SavePlayReport(PlayReportType type, u64 title_id, std::vector<std::vector<u8>> data,
                        std::optional<u64> process_id = {}, std::optional<u128> user_id = {}) const;

    void SaveErrorReport(u64 title_id, ResultCode result,
                         std::optional<std::string> custom_text_main = {},
                         std::optional<std::string> custom_text_detail = {}) const;

    void SaveFilesystemAccessReport(Service::FileSystem::LogMode log_mode,
                                    std::string log_message) const;

    void SaveUserReport() const;

private:
    bool IsReportingEnabled() const;

    System& system;
};

} // namespace Core
