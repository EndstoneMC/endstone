

# File map\_meta.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**meta**](dir_2d728641c8c30e7cdff7ab60efc98406.md) **>** [**map\_meta.h**](map__meta_8h.md)

[Go to the documentation of this file](map__meta_8h.md)


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
#include "endstone/map/map_view.h"

namespace endstone {
class MapMeta : public ItemMeta {
public:
    ENDSTONE_ITEM_META_TYPE(Map)

    using MapId = std::int64_t;
    [[nodiscard]] virtual bool hasMapId() const = 0;

    [[nodiscard]] virtual MapId getMapId() const = 0;

    virtual void setMapId(MapId id) = 0;

    [[nodiscard]] virtual bool hasMapView() const = 0;

    [[nodiscard]] virtual MapView *getMapView() const = 0;

    virtual void setMapView(const MapView *map) = 0;
};
}  // namespace endstone
```


