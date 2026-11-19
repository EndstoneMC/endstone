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
#include <string_view>
#include <typeinfo>
#include <utility>

#include "endstone/brigadier/arguments.h"

namespace endstone {

/**
 * An argument completed from a soft enum, whose value is the word typed.
 */
class SoftEnumArgumentType : public brigadier::ArgumentType {
public:
    explicit SoftEnumArgumentType(std::string name) : name_(std::move(name)) {}

    /**
     * Gets the name of the soft enum this argument completes from.
     *
     * @return the name of the soft enum
     */
    [[nodiscard]] const std::string &getName() const { return name_; }

    [[nodiscard]] brigadier::ArgumentValue parse(brigadier::StringReader &reader) const override
    {
        return reader.readString();
    }

    [[nodiscard]] std::string toString() const override { return "softEnum(" + name_ + ")"; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(SoftEnumArgumentType).name() ||
               brigadier::ArgumentType::isInstanceOf(target);
    }

private:
    std::string name_;
};

}  // namespace endstone
