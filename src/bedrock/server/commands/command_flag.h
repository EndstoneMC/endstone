
// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

using CommandFlagSize = std::uint16_t;

enum class CommandUsageFlag : CommandFlagSize {
    Normal = 0,
    Test = 1,
};

enum class CommandVisibilityFlag : CommandFlagSize {
    Visible = 0,
    HiddenFromCommandBlockOrigin = 2,
    HiddenFromPlayerOrigin = 4,
    HiddenFromAutomationOrigin = 8,
    Hidden = HiddenFromCommandBlockOrigin | HiddenFromPlayerOrigin,
    Removed = HiddenFromCommandBlockOrigin | HiddenFromPlayerOrigin | HiddenFromAutomationOrigin,
};

enum class CommandSyncFlag : CommandFlagSize {
    Synced = 0,
    Local = 16,
};

enum class CommandExecuteFlag : CommandFlagSize {
    Allowed = 0,
    Disallowed = 32,
};

enum class CommandTypeFlag : CommandFlagSize {
    None = 0,
    Message = 64,
};

enum class CommandCheatFlag : CommandFlagSize {
    Cheat = 0,
    NotCheat = 128,
};

enum class CommandAsyncFlag : CommandFlagSize {
    Synch = 0,
    Async = 256,
};

enum class CommandEditorFlag : CommandFlagSize {
    Editor = 0,
    NotEditor = 512,
};

struct CommandFlag {
    // NOLINTBEGIN(*-explicit-constructor)
    CommandFlag(CommandUsageFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    CommandFlag(CommandVisibilityFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    CommandFlag(CommandSyncFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    CommandFlag(CommandExecuteFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    CommandFlag(CommandTypeFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    CommandFlag(CommandCheatFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    CommandFlag(CommandAsyncFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    CommandFlag(CommandEditorFlag flag) : flag(static_cast<CommandFlagSize>(flag)) {}
    // NOLINTEND(*-explicit-constructor)
    explicit CommandFlag(CommandFlagSize flag) : flag(flag) {}
    CommandFlag() = default;

    CommandFlag operator|(const CommandFlag &other) const
    {
        return CommandFlag{static_cast<CommandFlagSize>(flag | other.flag)};
    }

    CommandFlag &operator|=(const CommandFlag &other)
    {
        flag = static_cast<CommandFlagSize>(flag | other.flag);
        return *this;
    }

    CommandFlag operator&(const CommandFlag &other) const
    {
        return CommandFlag{static_cast<CommandFlagSize>(flag & other.flag)};
    }

    bool operator==(const CommandFlag &other) const
    {
        return flag == other.flag;
    }

    [[nodiscard]] bool hasFlags(CommandFlag other) const
    {
        return (flag & other.flag) == other.flag;
    }

    CommandFlagSize flag;
};
