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

#include "bedrock/server/commands/standard/say_command.h"

#include <entt/entt.hpp>

#include "bedrock/locale/i18n.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"

using endstone::detail::EndstoneServer;

void SayCommand::_sendMessage(const std::string &message, const std::string &sender_name,
                              const CommandOriginIdentity &origin_identity, Level &level)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&SayCommand::_sendMessage, message, sender_name, origin_identity, level);
    auto &server = entt::locator<EndstoneServer>::value();
    server.getLogger().info(getI18n().get("chat.type.announcement", {sender_name, message}, nullptr));
}
