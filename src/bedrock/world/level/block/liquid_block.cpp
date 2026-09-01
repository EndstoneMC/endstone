#include "bedrock/world/level/block/liquid_block.h"

#include "bedrock/util/random.h"
#include "bedrock/world/level/block/components/block_liquid_detection_component.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"

void LiquidBlockBase::emitFizzParticle(BlockSource &region, BlockPos const &p) const
{
    if (!region.getPublicSource()) {
        return;
    }

    auto &level = region.getLevel();
    auto &random = level.getRandom();
    const auto a = random.nextFloat();
    const auto b = random.nextFloat();
    level.broadcastLocalEvent(
        region, LevelEvent::SoundFizz,
        {static_cast<float>(p.x) + 0.5F, static_cast<float>(p.y) + 0.5F, static_cast<float>(p.z) + 0.5F},
        static_cast<int>(((a - b) * 0.8F + 2.6F) * 1000.0F));

    for (int i = 0; i < 8; ++i) {
        const auto x = level.getRandom().nextFloat();
        const auto z = level.getRandom().nextFloat();
        level.addParticle(ParticleType::Smoke,
                          {static_cast<float>(p.x) + x, static_cast<float>(p.y) + 1.2F, static_cast<float>(p.z) + z},
                          Vec3::ZERO, 250, nullptr, false);
    }
}

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
