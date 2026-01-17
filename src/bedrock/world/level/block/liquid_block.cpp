#include "bedrock/world/level/block/liquid_block.h"

#include "bedrock/world/level/block/components/block_liquid_detection_component.h"
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
    return !_isLiquidBlocking(region, pos, flow_from_pos, flow_from_direction);
}

bool LiquidBlock::_isLiquidBlocking(BlockSource &region, BlockPos const &pos, BlockPos const &flow_from_pos,
                                    FacingID flow_from_direction) const
{
    if (region.getLiquidBlock(flow_from_pos).getMaterial().isType(MaterialType::Lava)) {
        return region.getBlock(pos).isLavaBlocking();
    }
    auto get_block = [&region](const BlockPos &block_pos) -> const Block & {
        return region.getBlock(block_pos);
    };
    if (BlockLiquidDetectionComponent::isLiquidBlocking(region.getBlock(pos))) {
        return true;
    }
    if (!BlockLiquidDetectionComponent::liquidCanFlowIntoFromDirection(region.getBlock(pos), flow_from_direction,
                                                                       get_block, flow_from_pos)) {
        return true;
    }
    return !BlockLiquidDetectionComponent::liquidCanFlowIntoFromDirection(
        region.getBlock(flow_from_pos), Facing::OPPOSITE_FACING[flow_from_direction], get_block, flow_from_pos);
}
