

# File attribute\_modifier.h

[**File List**](files.md) **>** [**attribute**](dir_5bd322557a2ad4dbfaf5ca599d780f7c.md) **>** [**attribute\_modifier.h**](attribute__modifier_8h.md)

[Go to the documentation of this file](attribute__modifier_8h.md)


```C++
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

#include "endstone/identifier.h"

namespace endstone {
using AttributeModifierId = Identifier<class AttributeModifier>;

class AttributeModifier {
public:
    enum class Operation {
        Add,
        MultiplyBase,
        Multiply,
        Cap
    };

    AttributeModifier(AttributeModifierId id, float amount, Operation operation)
        : id_(std::string(id)), amount_(amount), operation_(operation)
    {
    }

    [[nodiscard]] AttributeModifierId getId() const { return AttributeModifierId{id_}; }

    [[nodiscard]] float getAmount() const { return amount_; }

    [[nodiscard]] Operation getOperation() const { return operation_; }

private:
    std::string id_;
    float amount_;
    Operation operation_;
};
}  // namespace endstone
```


