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

#include "endstone/brigadier/arguments.h"
#include "endstone/brigadier/exceptions.h"

namespace endstone {

/**
 * A position of three decimal, relative or local coordinates, resolved through a FinePositionResolver.
 */
class FinePositionArgumentType : public brigadier::ArgumentType {
public:
    [[nodiscard]] brigadier::ArgumentValue parse(brigadier::StringReader &reader) const override
    {
        static const brigadier::SimpleCommandExceptionType type{"A position is parsed by the server."};
        throw type.createWithContext(reader);
    }

    [[nodiscard]] std::string toString() const override { return "finePosition()"; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(FinePositionArgumentType).name() ||
               brigadier::ArgumentType::isInstanceOf(target);
    }
};

}  // namespace endstone
