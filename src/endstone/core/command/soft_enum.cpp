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

#include "endstone/core/command/soft_enum.h"

#include <utility>

#include "endstone/core/command/command_map.h"

namespace endstone::core {

EndstoneSoftEnum::EndstoneSoftEnum(EndstoneCommandMap &command_map, std::string name)
    : command_map_(command_map), name_(std::move(name))
{
}

std::string EndstoneSoftEnum::getName() const
{
    return name_;
}

std::vector<std::string> EndstoneSoftEnum::getValues() const
{
    return command_map_.getSoftEnumValues(*this);
}

void EndstoneSoftEnum::setValues(std::vector<std::string> values)
{
    command_map_.updateSoftEnum(*this, SoftEnumUpdateType::Replace, std::move(values));
}

void EndstoneSoftEnum::addValues(std::vector<std::string> values)
{
    command_map_.updateSoftEnum(*this, SoftEnumUpdateType::Add, std::move(values));
}

void EndstoneSoftEnum::removeValues(std::vector<std::string> values)
{
    command_map_.updateSoftEnum(*this, SoftEnumUpdateType::Remove, std::move(values));
}

}  // namespace endstone::core
