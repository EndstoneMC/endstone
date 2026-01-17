#include "bedrock/world/level/block/liquid_block.h"

#include "bedrock/world/level/block_source.h"

bool LiquidBlock::_canSpreadTo(BlockSource &region, BlockPos const &pos, BlockPos const &flow_from_pos,
                               FacingID flow_from_direction) const
{
    if (pos.y < region.getMinHeight()) {
        return false;
    }
    if (!region.hasBlock(pos)) {
        return false;
    }
    const auto &block = region.getLiquidBlock(pos);
    if (block.getMaterial() == getMaterial() || block.getMaterial().isType(MaterialType::Lava)) {
        return false;
    }
    return _isLiquidBlocking(region, pos, flow_from_pos, flow_from_direction);
}
