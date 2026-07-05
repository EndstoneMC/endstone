#include "bedrock/world/level/block/leaves_block.h"

#include <array>

#include "bedrock/world/level/block/resource_drops_context.h"
#include "bedrock/world/level/block/states/vanilla_block_states.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/phys/aabb.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_type.h"
#include "endstone/core/server.h"
#include "endstone/event/block/leaves_decay_event.h"

#ifdef _WIN32
void LeavesBlock::randomTick(BlockEvents::BlockRandomTickEvent &event_data) const
#elif __linux__
void LeavesBlock::randomTick(BlockEvents::BlockRandomTickEvent &event_data)
#endif
{
    auto &region = event_data.region;
    const auto &pos = event_data.pos;
    const auto &block = region.getBlock(pos);

    bool update_bit = block.getState<bool>(VanillaStateIds::UpdateBit);
    bool persistent_bit = block.getState<bool>(VanillaStateIds::PersistentBit);

    if (update_bit && !persistent_bit) {
        constexpr int r = REQUIRED_WOOD_RANGE;
        constexpr int r2 = r + 1;
        constexpr int W = r2 * 2 + 1;
        constexpr int WW = W * W;
        constexpr int WO = W / 2;

        std::array<std::int8_t, W * W * W> check_buffer{};

        AABB aabb(static_cast<float>(pos.x - r2), static_cast<float>(pos.y - r2), static_cast<float>(pos.z - r2),
                  static_cast<float>(pos.x + r2), static_cast<float>(pos.y + r2), static_cast<float>(pos.z + r2));

        if (region.hasChunksAt(aabb, false)) {
            for (int xo = -r; xo <= r; xo++) {
                for (int yo = -r; yo <= r; yo++) {
                    for (int zo = -r; zo <= r; zo++) {
                        const auto &nearby = region.getBlock({pos.x + xo, pos.y + yo, pos.z + zo});
                        if (nearby.getBlockType().anyOf(VanillaBlockTypeGroups::LogAndStrippedLogBlockIds) ||
                            nearby.getBlockType().anyOf(VanillaBlockTypeGroups::WoodAndStrippedWoodBlockIds)) {
                            check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] = 0;
                        }
                        else if (nearby.hasProperty(BlockProperty::Leaves)) {
                            check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] = -2;
                        }
                        else {
                            check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] = -1;
                        }
                    }
                }
            }

            for (int i = 1; i <= REQUIRED_WOOD_RANGE; i++) {
                for (int xo = -r; xo <= r; xo++) {
                    for (int yo = -r; yo <= r; yo++) {
                        for (int zo = -r; zo <= r; zo++) {
                            if (check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] == i - 1) {
                                if (check_buffer[(xo + WO - 1) * WW + (yo + WO) * W + (zo + WO)] == -2) {
                                    check_buffer[(xo + WO - 1) * WW + (yo + WO) * W + (zo + WO)] = i;
                                }
                                if (check_buffer[(xo + WO + 1) * WW + (yo + WO) * W + (zo + WO)] == -2) {
                                    check_buffer[(xo + WO + 1) * WW + (yo + WO) * W + (zo + WO)] = i;
                                }
                                if (check_buffer[(xo + WO) * WW + (yo + WO - 1) * W + (zo + WO)] == -2) {
                                    check_buffer[(xo + WO) * WW + (yo + WO - 1) * W + (zo + WO)] = i;
                                }
                                if (check_buffer[(xo + WO) * WW + (yo + WO + 1) * W + (zo + WO)] == -2) {
                                    check_buffer[(xo + WO) * WW + (yo + WO + 1) * W + (zo + WO)] = i;
                                }
                                if (check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO - 1)] == -2) {
                                    check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO - 1)] = i;
                                }
                                if (check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO + 1)] == -2) {
                                    check_buffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO + 1)] = i;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (check_buffer[WO * WW + WO * W + WO] < 0) {
            _die(region, pos);
        }
        else {
            auto new_block = block.setState<bool>(VanillaStateIds::UpdateBit, false);
            region.setBlock(pos, *new_block, UPDATE_INVISIBLE, nullptr, {});
        }
    }
}

void LeavesBlock::_die(BlockSource &region, const BlockPos &pos) /*const*/
{
    // Endstone begins
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::LeavesDecayEvent event(endstone::core::EndstoneBlock::at(region, pos));
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }
    // Endstone ends

    auto &random = region.getLevel().getRandom();
    const auto &block = region.getBlock(pos);
    block.spawnResources(region, pos, random, ResourceDropsContext::fromOtherCause(region, pos), nullptr);
    const auto &air = static_cast<const endstone::core::EndstoneBlockType &>(
                          server.getRegistry<endstone::BlockType>().getOrThrow(endstone::BlockType::Air))
                          .getHandle()
                          .getDefaultState();
    region.setBlock(pos, air, UPDATE_ALL, nullptr, {});
}
