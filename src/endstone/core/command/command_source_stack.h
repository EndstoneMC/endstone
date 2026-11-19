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

#include <memory>

#include "bedrock/server/commands/command_origin.h"
#include "endstone/actor/actor.h"
#include "endstone/command/brigadier/command_source_stack.h"
#include "endstone/command/command_sender.h"
#include "endstone/level/location.h"

namespace endstone::core {

class EndstoneCommandSourceStack : public CommandSourceStack {
public:
    EndstoneCommandSourceStack(NotNull<CommandSender> sender, Nullable<Actor> executor, Location location,
                               std::shared_ptr<const ::CommandOrigin> origin = nullptr);
    [[nodiscard]] NotNull<CommandSender> getSender() const override;
    [[nodiscard]] Nullable<Actor> getExecutor() const override;
    [[nodiscard]] Location getLocation() const override;
    [[nodiscard]] const ::CommandOrigin *getOrigin() const;

private:
    NotNull<CommandSender> sender_;
    Nullable<Actor> executor_;
    Location location_;
    std::shared_ptr<const ::CommandOrigin> origin_;
};

}  // namespace endstone::core
