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

#include "endstone/core/block/block_actor_state.h"

#include "bedrock/dataloadhelper/data_load_helper.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/item/save_context.h"
#include "bedrock/world/level/block/actor/block_actor.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor_factory.h"
#include "endstone/check.h"

namespace endstone::core {

namespace {

class SnapshotDataLoadHelper final : public DataLoadHelper {
public:
    Vec3 loadPosition(const Vec3 &position) override { return position; }
    BlockPos loadBlockPosition(const BlockPos &block_pos) override { return block_pos; }
    BlockPos loadBlockPositionOffset(const BlockPos &block_pos_offset) override { return block_pos_offset; }
    float loadRotationDegreesX(float value) override { return value; }
    float loadRotationDegreesY(float value) override { return value; }
    float loadRotationRadiansX(float value) override { return value; }
    float loadRotationRadiansY(float value) override { return value; }
    std::uint8_t loadFacingID(std::uint8_t facing) override { return facing; }
    Vec3 loadDirection(const Vec3 &direction) override { return direction; }
    Direction::Type loadDirection(Direction::Type direction) override { return direction; }
    Rotation loadRotation(Rotation rotation) override { return rotation; }
    Mirror loadMirror(Mirror mirror) override { return mirror; }
    ActorUniqueID loadActorUniqueID(ActorUniqueID id) override { return id; }
    ActorUniqueID loadOwnerID(ActorUniqueID id) override { return id; }
    const InternalComponentRegistry::ComponentInfo *loadActorInternalComponentInfo(
        const std::unordered_map<HashedString, InternalComponentRegistry::ComponentInfo> &,
        const std::string &) override
    {
        return nullptr;
    }
    DataLoadHelperType getType() const override { return DataLoadHelperType::NewUniqueIds; }
    bool shouldResetTime() override { return false; }
};

std::shared_ptr<::BlockActor> cloneBlockActor(::ILevel &level, const ::BlockActor &source, const ::BlockPos &position,
                                              const ::BlockType &block)
{
    ::CompoundTag tag;
    if (!source.save(tag, SaveContext::forClone())) {
        return nullptr;
    }

    auto clone = VanillaBlockActorFactory::createBlockActor(position, block);
    if (clone == nullptr) {
        return nullptr;
    }

    SnapshotDataLoadHelper data_load_helper;
    clone->load(level, tag, data_load_helper);
    return clone;
}

}  // namespace

EndstoneBlockActorState::~EndstoneBlockActorState() = default;

void EndstoneBlockActorState::initializeBlockActor(::ILevel &level, ::BlockActor &block_actor,
                                                   const ::BlockPos &position, const ::BlockType &block,
                                                   bool use_snapshot)
{
    block_actor_ = &block_actor;
    if (!use_snapshot) {
        return;
    }

    snapshot_ = cloneBlockActor(level, block_actor, position, block);
    Preconditions::checkState(snapshot_ != nullptr, "Unable to create a block entity snapshot.");
    block_actor_ = snapshot_.get();
}

::BlockActor *EndstoneBlockActorState::getBlockActor() const
{
    return block_actor_;
}

bool EndstoneBlockActorState::isSnapshot() const
{
    return snapshot_ != nullptr;
}

bool EndstoneBlockActorState::applyTo(::ILevel &level, ::BlockActor &block_actor) const
{
    if (isSnapshot()) {
        ::CompoundTag tag;
        if (!serializeForUpdate(tag)) {
            return false;
        }

        SnapshotDataLoadHelper data_load_helper;
        block_actor.load(level, tag, data_load_helper);
    }
    static_cast<::VanillaBlockActor &>(block_actor).setChanged();
    return true;
}

bool serializeBlockActor(const ::BlockActor &block_actor, ::CompoundTag &tag)
{
    return block_actor.save(tag, SaveContext::forNetwork());
}

}  // namespace endstone::core
