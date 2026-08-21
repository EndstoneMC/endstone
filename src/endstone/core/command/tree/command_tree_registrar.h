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

#include <string>
#include <vector>

#include <nonstd/expected.hpp>

#include "bedrock/server/commands/command_registry.h"
#include "endstone/command/argument_type.h"

namespace endstone::core {

class TreeCommand;

/**
 * Turns command tree nodes into the parameters the command registry accepts.
 *
 * Reaches into the registry's enum tables, which is why it is a friend of CommandRegistry.
 */
class CommandTreeRegistrar {
public:
    /**
     * Builds the parameter a literal node is registered as.
     *
     * A literal becomes a single-value enum, which is what the client renders as the word to type.
     */
    static nonstd::expected<CommandParameterData, std::string> makeLiteralParameter(const std::string &literal,
                                                                                    TreeCommand &command,
                                                                                    CommandRegistry &registry);

    /**
     * Builds the parameter an argument node is registered as.
     *
     * Returns an error message for an argument type that cannot yet be registered.
     */
    static nonstd::expected<CommandParameterData, std::string> makeArgumentParameter(const std::string &name,
                                                                                     const ArgumentType &type,
                                                                                     TreeCommand &command,
                                                                                     CommandRegistry &registry);

    /**
     * Replaces the values of a soft enum, creating it if it does not exist yet.
     *
     * @return true if the values were pushed to connected clients
     */
    static bool setSoftEnumValues(const std::string &name, std::vector<std::string> values, CommandRegistry &registry);

private:
    static CommandParameterData basicParameter(const std::string &name, TreeCommand &command);
    static nonstd::expected<CommandParameterData, std::string> existingEnumParameter(const std::string &name,
                                                                                     const char *enum_name,
                                                                                     TreeCommand &command,
                                                                                     CommandRegistry &registry);
    static nonstd::expected<CommandParameterData, std::string> newEnumParameter(const std::string &name,
                                                                                const std::string &wanted_enum_name,
                                                                                const std::vector<std::string> &values,
                                                                                TreeCommand &command,
                                                                                CommandRegistry &registry);
    static nonstd::expected<CommandParameterData, std::string> softEnumParameter(const std::string &name,
                                                                                 const std::string &enum_name,
                                                                                 TreeCommand &command,
                                                                                 CommandRegistry &registry);
    static nonstd::expected<CommandParameterData, std::string> hardNonTerminalParameter(
        const std::string &name, CommandRegistry::HardNonTerminal symbol, TreeCommand &command);
    static std::uint32_t findOrCreateSoftEnum(const std::string &name, CommandRegistry &registry);
    static std::string uniqueEnumName(const std::string &wanted, const CommandRegistry &registry);
};

}  // namespace endstone::core
