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

#include "endstone/core/inventory/item_metas.h"

#include "endstone/inventory/meta/item_meta.h"
#include "endstone/inventory/meta/map_meta.h"

namespace endstone::core {
EndstoneItemMetas::ItemMetaDetails EndstoneItemMetas::EMPTY = ItemMetaDetails::create<void>();
EndstoneItemMetas::ItemMetaDetails EndstoneItemMetas::ITEM = ItemMetaDetails::create<ItemMeta>();
EndstoneItemMetas::ItemMetaDetails EndstoneItemMetas::MAP = ItemMetaDetails::create<MapMeta>();
}  // namespace endstone::core
