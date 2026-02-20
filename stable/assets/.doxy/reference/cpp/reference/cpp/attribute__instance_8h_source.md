

# File attribute\_instance.h

[**File List**](files.md) **>** [**attribute**](dir_5bd322557a2ad4dbfaf5ca599d780f7c.md) **>** [**attribute\_instance.h**](attribute__instance_8h.md)

[Go to the documentation of this file](attribute__instance_8h.md)


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

#include <vector>

#include "endstone/attribute/attribute_modifier.h"

namespace endstone {
class AttributeInstance {
public:
    virtual ~AttributeInstance() = default;

    [[nodiscard]] virtual AttributeId getType() const = 0;

    [[nodiscard]] virtual float getBaseValue() const = 0;

    virtual void setBaseValue(float value) = 0;

    [[nodiscard]] virtual float getValue() const = 0;

    [[nodiscard]] virtual std::vector<AttributeModifier> getModifiers() const = 0;

    virtual void addModifier(const AttributeModifier &modifier) = 0;

    virtual void removeModifier(const AttributeModifier &modifier) = 0;
};
}  // namespace endstone
```


