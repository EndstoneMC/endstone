

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

#include "endstone/detail.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/map/map_view.h"

namespace endstone {
class MapMeta final : public ItemMeta {
public:
    explicit MapMeta(const ItemMeta *meta) : ItemMeta(meta)
    {
        const auto *map = meta ? meta->as<MapMeta>() : nullptr;
        if (!map) {
            return;
        }
        map_id_ = map->map_id_;
    }

    static constexpr auto TYPE = Type::Map;
    [[nodiscard]] Type getType() const override
    {
        return TYPE;
    }

    [[nodiscard]] bool isEmpty() const override
    {
        return ItemMeta::isEmpty() && isMapEmpty();
    }

    [[nodiscard]] std::unique_ptr<ItemMeta> clone() const override
    {
        return std::make_unique<MapMeta>(this);
    }

    [[nodiscard]] bool hasMapId() const
    {
        return map_id_ != -1;
    }

    [[nodiscard]] std::int64_t getMapId() const
    {
        return map_id_;
    }

    void setMapId(std::int64_t id)
    {
        map_id_ = id;
    }

    [[nodiscard]] bool hasMapView() const
    {
        return hasMapId();
    }

    [[nodiscard]] MapView *getMapView() const
    {
        return detail::getServer().getMap(getMapId());
    }

    void setMapView(const MapView *map)
    {
        map_id_ = map ? map->getId() : -1;
    }

protected:
    [[nodiscard]] bool isMapEmpty() const
    {
        return !(hasMapId());  // TODO(map): add hasScaling
    }

private:
    std::int64_t map_id_ = -1;
};
}  // namespace endstone
```


