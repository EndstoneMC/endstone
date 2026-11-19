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

#include "endstone/util/pointers.h"

namespace endstone {

class Actor;
class CommandSender;
class Location;

/**
 * The source of a command run through a Brigadier command tree, carrying who ran it and where.
 */
class CommandSourceStack {
public:
    virtual ~CommandSourceStack() = default;

    /**
     * Gets the command sender that triggered the command.
     *
     * @return the command sender
     */
    [[nodiscard]] virtual NotNull<CommandSender> getSender() const = 0;

    /**
     * Gets the actor that executes the command, which differs from the sender after `/execute as`.
     *
     * @return the executing actor, or a null handle if no actor executes the command
     */
    [[nodiscard]] virtual Nullable<Actor> getExecutor() const = 0;

    /**
     * Gets a copy of the location where the command is executed.
     *
     * @return the location of the command
     */
    [[nodiscard]] virtual Location getLocation() const = 0;
};

/**
 * The source type of every Brigadier node in a command tree.
 */
using CommandSource = NotNull<CommandSourceStack>;

}  // namespace endstone
