

# File creature\_spawner.h

[**File List**](files.md) **>** [**block**](dir_faca67fc60a7463eb1bd30eabe023cf1.md) **>** [**creature\_spawner.h**](creature__spawner_8h.md)

[Go to the documentation of this file](creature__spawner_8h.md)


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

#include "endstone/actor/actor_type.h"
#include "endstone/block/block_state.h"

namespace endstone {

class CreatureSpawner : public BlockState {
public:
    [[nodiscard]] virtual const ActorType &getSpawnedType() const = 0;

    virtual void setSpawnedType(ActorTypeId type) = 0;

    [[nodiscard]] virtual int getDelay() const = 0;

    virtual void setDelay(int delay) = 0;

    [[nodiscard]] virtual int getMinSpawnDelay() const = 0;

    virtual void setMinSpawnDelay(int delay) = 0;

    [[nodiscard]] virtual int getMaxSpawnDelay() const = 0;

    virtual void setMaxSpawnDelay(int delay) = 0;

    [[nodiscard]] virtual int getSpawnCount() const = 0;

    virtual void setSpawnCount(int count) = 0;

    [[nodiscard]] virtual int getMaxNearbyEntities() const = 0;

    virtual void setMaxNearbyEntities(int count) = 0;

    [[nodiscard]] virtual int getRequiredPlayerRange() const = 0;

    virtual void setRequiredPlayerRange(int range) = 0;

    [[nodiscard]] virtual int getSpawnRange() const = 0;

    virtual void setSpawnRange(int range) = 0;
};

}  // namespace endstone
```


