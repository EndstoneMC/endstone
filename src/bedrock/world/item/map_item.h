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

#include "bedrock/world/item/complex_item.h"

class MapItem : public ComplexItem {
public:
    inline static const std::string TAG_MAP_SCALING = "map_is_scaling";
    inline static const std::string TAG_MAP_SCALE = "map_scale";
    inline static const std::string TAG_MAP_PLAYER_DISPLAY = "map_display_player";
    inline static const std::string TAG_MAP_UUID = "map_uuid";
    inline static const std::string TAG_MAP_NAME_INDEX = "map_name_index";
    inline static const std::string TAG_MAP_INIT = "map_is_init";
    inline static const std::string TAG_MAP_REGENERATE = "map_regenerate";

    using ComplexItem::ComplexItem;
};
