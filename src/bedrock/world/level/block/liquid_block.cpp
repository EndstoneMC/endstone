#include "bedrock/world/level/block/liquid_block.h"

#include "bedrock/world/level/block_source.h"

bool LiquidBlock::_canSpreadTo(BlockSource &region, BlockPos const &pos, BlockPos const &flow_from_pos,
                               unsigned char flow_from_direction) const
{
    if (pos.y < region.getMinHeight()) {
        return false;
    }
    if (const auto &block = region.getLiquidBlock(pos);
        block.getMaterial() == getMaterial() || block.getMaterial().isType(MaterialType::Lava) ||
        _isLiquidBlocking(region, pos, flow_from_pos, flow_from_direction)) {
        return false;
    }
    return true;
}
