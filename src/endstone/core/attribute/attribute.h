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

#include <span>
#include <string>

#include "endstone/attribute/attribute.h"

namespace endstone::core {

class EndstoneAttribute : public Attribute {
public:
    explicit EndstoneAttribute(AttributeId id);
    [[nodiscard]] AttributeId getId() const override;
    [[nodiscard]] std::string getTranslationKey() const override;

    static std::span<const AttributeId> all();

private:
    AttributeId id_;
};

}  // namespace endstone::core
