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

#include <functional>
#include <memory>
#include <utility>

namespace endstone {

class CommandSourceStack;

/**
 * Resolves the value of an argument against the source of a command.
 *
 * @tparam T the resolved type
 */
template <typename T>
class ArgumentResolver {
public:
    explicit ArgumentResolver(std::function<T(const CommandSourceStack &)> resolver)
        : resolver_(std::make_shared<const std::function<T(const CommandSourceStack &)>>(std::move(resolver)))
    {
    }

    /**
     * Resolves the argument with the given command source stack.
     *
     * @param source the source of the command
     * @return the resolved value
     * @throws brigadier::CommandSyntaxException if the argument does not resolve
     * @throws std::invalid_argument if the source was not created by the server
     */
    [[nodiscard]] T resolve(const CommandSourceStack &source) const { return (*resolver_)(source); }

    /**
     * Checks whether two resolvers are copies of one resolver.
     *
     * @param other the other resolver
     * @return true if both are copies of one resolver
     */
    bool operator==(const ArgumentResolver &other) const { return resolver_ == other.resolver_; }

private:
    std::shared_ptr<const std::function<T(const CommandSourceStack &)>> resolver_;
};

}  // namespace endstone
