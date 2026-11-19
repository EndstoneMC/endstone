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

#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>

#include <gtest/gtest.h>

#include "bedrock/server/commands/minecraft_commands.h"

template <typename Tag, typename Tag::type Member>
struct Expose {
    friend constexpr typename Tag::type member(Tag) { return Member; }
};

struct ChatPermissionsCallback {
    using type = std::function<bool()> MinecraftCommands::*;
    friend constexpr type member(ChatPermissionsCallback);
};
template struct Expose<ChatPermissionsCallback, &MinecraftCommands::chat_permissions_callback_>;

struct CompiledCommandMap {
    using type = std::unordered_map<HashedString, std::unique_ptr<Command>> MinecraftCommands::*;
    friend constexpr type member(CompiledCommandMap);
};
template struct Expose<CompiledCommandMap, &MinecraftCommands::compiled_command_map_>;

TEST(MinecraftCommandsTest, MemberOffsets)
{
    alignas(MinecraftCommands) unsigned char storage[sizeof(MinecraftCommands)];
    auto *commands = reinterpret_cast<MinecraftCommands *>(storage);
    const auto offset = [&](auto pointer) -> std::ptrdiff_t {
        return reinterpret_cast<unsigned char *>(&(commands->*pointer)) - storage;
    };
#ifdef _WIN32
    EXPECT_EQ(offset(member(ChatPermissionsCallback{})), 40);
    EXPECT_EQ(offset(member(CompiledCommandMap{})), 104);
#else
    EXPECT_EQ(offset(member(ChatPermissionsCallback{})), 48);
    EXPECT_EQ(offset(member(CompiledCommandMap{})), 96);
#endif
}
