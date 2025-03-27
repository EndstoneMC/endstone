

# File item\_factory.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**item\_factory.h**](item__factory_8h.md)

[Go to the documentation of this file](item__factory_8h.md)


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

#include "endstone/inventory/meta/item_meta.h"
#include "endstone/inventory/meta/map_meta.h"

namespace endstone {
class ItemFactory {
public:
    static std::unique_ptr<ItemMeta> getItemMeta(const std::string_view type, const ItemMeta *meta = nullptr)
    {
        if (type == "minecraft:filled_map") {
            return std::make_unique<MapMeta>(meta);
        }
        return std::make_unique<ItemMeta>(meta);
    }

    static std::unique_ptr<ItemMeta> asMetaFor(const std::string_view type, const ItemMeta *meta)
    {
        return getItemMeta(type, meta);
    }
};
}  // namespace endstone
```


