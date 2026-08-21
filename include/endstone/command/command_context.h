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

#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "endstone/command/argument_type.h"
#include "endstone/command/command_error.h"
#include "endstone/command/command_sender.h"
#include "endstone/util/pointers.h"

namespace endstone {

class Command;

/**
 * The arguments and sender a command handler is invoked with.
 */
class CommandContext {
public:
    virtual ~CommandContext() = default;

    /**
     * Returns the source of this command.
     *
     * @return Sender that ran the command
     */
    [[nodiscard]] virtual const NotNull<CommandSender> &getSender() const = 0;

    /**
     * Returns the command being executed.
     *
     * @return Command which was executed
     */
    [[nodiscard]] virtual const Command &getCommand() const = 0;

    /**
     * Returns the names of the arguments bound on the branch that ran.
     *
     * @return Bound argument names, in the order they were parsed
     */
    [[nodiscard]] virtual std::vector<std::string> getArgumentNames() const = 0;

    /**
     * @internal Prefer get() and getOptional().
     */
    [[nodiscard]] virtual const ArgumentValue *find(std::string_view name) const = 0;

    /**
     * Returns whether the named argument was provided on the branch that ran.
     *
     * @param name Name the argument was declared with
     * @return true if the argument has a value, otherwise false
     */
    [[nodiscard]] bool has(std::string_view name) const { return find(name) != nullptr; }

    /**
     * Returns the value of the named argument.
     *
     * @tparam T Type the argument was declared to produce
     * @param name Name the argument was declared with
     * @return Value of the argument
     * @throws CommandError if the argument is absent on this branch or is of another type
     */
    template <typename T>
    [[nodiscard]] T get(std::string_view name) const
    {
        const auto *value = find(name);
        if (!value) {
            throw CommandError(
                std::format("No argument '{}' on this command. Available: {}.", name, formatArgumentNames()));
        }
        const auto *typed = value->get<T>();
        if (!typed) {
            throw CommandError(std::format("Argument '{}' is not of the requested type.", name));
        }
        return *typed;
    }

    /**
     * Returns the value of the named argument, or nothing if it was omitted.
     *
     * @tparam T Type the argument was declared to produce
     * @param name Name the argument was declared with
     * @return Value of the argument, or std::nullopt if it is absent on this branch
     * @throws CommandError if the argument is present but of another type
     */
    template <typename T>
    [[nodiscard]] std::optional<T> getOptional(std::string_view name) const
    {
        if (!find(name)) {
            return std::nullopt;
        }
        return get<T>(name);
    }

private:
    [[nodiscard]] std::string formatArgumentNames() const
    {
        std::string result;
        for (const auto &name : getArgumentNames()) {
            if (!result.empty()) {
                result += ", ";
            }
            result += name;
        }
        return result.empty() ? "none" : result;
    }
};

}  // namespace endstone
