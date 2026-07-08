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

#include "bedrock/world/level/block/resource_drops_context.h"

#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block_source.h"

ResourceDropsContext::ResourceDropsContext(ResourceDropsCause cause, float explosion_radius, const ItemStack &used_item,
                                           const IBlockSource &block_source, const BlockPos &block_pos,
                                           DimensionType dimension_type)
    : cause(cause), explosion_radius(explosion_radius), used_item(used_item), block_pos(block_pos),
      dimension_type(dimension_type), block_source_(block_source)
{
}

ResourceDropsContext ResourceDropsContext::fromOtherCause(const IBlockSource &block_source, const BlockPos &position)
{
    return {ResourceDropsCause::Other,    1.0, ItemStack::EMPTY_ITEM, block_source, position,
            block_source.getDimensionId()};
}
