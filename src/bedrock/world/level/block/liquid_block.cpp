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
    const auto &material = region.getLiquidBlock(pos).getMaterial();
    if (material == getMaterial() || material.isType(MaterialType::Lava)) {
        return false;
    }
    return !_isLiquidBlocking(region, pos, flow_from_pos, flow_from_direction);
}

bool LiquidBlock::_isLiquidBlocking(BlockSource &region, BlockPos const &pos, BlockPos const &flow_from_pos,
                                    FacingID flow_from_direction) const
{
    const auto &to_block = region.getBlock(pos);
    if (region.getLiquidBlock(flow_from_pos).getMaterial().isType(MaterialType::Lava)) {
        return to_block.isLavaBlocking();
    }
    if (BlockLiquidDetectionComponent::isLiquidBlocking(to_block)) {
        return true;
    }
    auto get_block = [&region](const BlockPos &block_pos) -> const Block & {
        return region.getBlock(block_pos);
    };
    if (!BlockLiquidDetectionComponent::liquidCanFlowIntoFromDirection(to_block, flow_from_direction, get_block, pos)) {
        return true;
    }
    return !BlockLiquidDetectionComponent::liquidCanFlowIntoFromDirection(
        region.getBlock(flow_from_pos), Facing::OPPOSITE_FACING[flow_from_direction], get_block, flow_from_pos);
}
