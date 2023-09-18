// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <memory>
#include <string>

#include "endstone/logger.h"

class PluginCommand;
class CommandSender;

class Server {
public:
    Server(const Server &) = delete;             // Disable copy constructor
    Server &operator=(const Server &) = delete;  // Disable assignment operator

    Server() = default;
    virtual ~Server() = default;

    /**
     * Returns the primary logger associated with this server instance.
     *
     * @return Logger associated with this server
     */
    virtual Logger &getLogger() = 0;

    /**
     * Gets a PluginCommand with the given name or alias.
     *
     * @param name the name of the command to retrieve
     * @return a plugin command if found, null otherwise
     */
    virtual PluginCommand *getPluginCommand(const std::string &name) = 0;

    /**
     * Dispatches a command on this server, and executes it if found.
     *
     * @param sender the apparent sender of the command
     * @param commandLine the command + arguments. Example: <code>test abc 123</code>
     * @return returns false if no target is found
     * @throws std::exception thrown when the executor for the given command
     *     fails with an unhandled exception
     */
    virtual bool dispatchCommand(CommandSender &sender, const std::string &command_line) = 0;

    /**
     * Gets a ConsoleCommandSender that may be used as an input source
     * for this server.
     *
     * @return a console command sender
     */
    [[maybe_unused]] virtual CommandSender &getConsoleSender() = 0;
};
