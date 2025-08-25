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

#include "bedrock/world/level/block/block.h"

#include "bedrock/world/level/block_source.h"

bool Block::hasProperty(BlockProperty property) const
{
    return block_type_->hasProperty(property);
}

Brightness Block::getLightEmission() const
{
    return direct_data_.light_emission;
}

float Block::getTranslucency() const
{
    return block_type_->getTranslucency();
}

bool Block::isSolid() const
{
    return block_type_->isSolid();
}
Brightness Block::getLight() const
{
    return direct_data_.light;
}

int Block::getFlameOdds() const
{
    return direct_data_.flame_odds;
}

int Block::getBurnOdds() const
{
    return direct_data_.burn_odds;
}

float Block::getExplosionResistance() const
{
    return direct_data_.explosion_resistance;
}

bool Block::hasState(const BlockState &block_state) const
{
    return getBlockType().hasState(block_state);
}

bool Block::hasState(const HashedString &name) const
{
    return getBlockType().hasState(name);
}

bool Block::getCollisionShape(AABB &out_aabb, IConstBlockSource const &region, BlockPos const &pos,
                              optional_ref<GetCollisionShapeInterface const> entity) const
{
    out_aabb = block_type_->getCollisionShape(*this, region, pos, entity);
    return out_aabb.min.x < out_aabb.max.x && out_aabb.min.y < out_aabb.max.y && out_aabb.min.z < out_aabb.max.z;
}

bool Block::addCollisionShapes(IConstBlockSource const &region, BlockPos const &pos, AABB const *intersect_test_box,
                               std::vector<AABB> &in_out_boxes,
                               optional_ref<GetCollisionShapeInterface const> entity) const
{
    return block_type_->addCollisionShapes(*this, region, pos, intersect_test_box, in_out_boxes, entity);
}

bool Block::getCollisionShapeForCamera(AABB &out_aabb, IConstBlockSource const &region, BlockPos const &pos) const
{
    return block_type_->getCollisionShapeForCamera(out_aabb, *this, region, pos);
}

const AABB &Block::getOutline(IConstBlockSource const &region, BlockPos const &pos, AABB &buffer) const
{
    return block_type_->getOutline(*this, region, pos, buffer);
}

const AABB &Block::getVisualShape(AABB &buffer) const
{
    return block_type_->getVisualShape(*this, buffer);
}

const AABB &Block::getUIShape(AABB &buffer) const
{
    return block_type_->getUIShape(*this, buffer);
}

bool Block::getLiquidClipVolume(BlockSource &region, BlockPos const &pos, AABB &include_box) const
{
    return block_type_->getLiquidClipVolume(*this, region, pos, include_box);
}

bool Block::requiresCorrectToolForDrops() const
{
    return block_type_->requiresCorrectToolForDrops();
}

float Block::getThickness() const
{
    return block_type_->getThickness();
}

bool Block::getSecondPart(const BlockSource &region, const BlockPos &pos, BlockPos &out) const
{
    return block_type_->getSecondPart(region, pos, out);
}

void Block::destroy(BlockSource &region, const BlockPos &pos, Actor *entity_source) const
{
    getBlockType().destroy(region, pos, *this, entity_source);
}

const Material &Block::getMaterial() const
{
    return getBlockType().getMaterial();
}

float Block::getFriction() const
{
    return direct_data_.friction;
}

float Block::getDestroySpeed() const
{
    return direct_data_.destroy_speed;
}

const HashedString &Block::getName() const
{
    return block_type_->getName();
}

const CompoundTag &Block::getSerializationId() const
{
    return serialization_id_;
}

BlockRuntimeId Block::getRuntimeId() const
{
    return network_id_;
}

std::string Block::toDebugString() const
{
    std::stringstream ss;
    ss << "name: " << serialization_id_.getString("name");
    return ss.str();
}

const BlockType &Block::getBlockType() const
{
    return *block_type_;
}

const std::vector<HashedString> &Block::getTags() const
{
    if (!tags_.empty()) {
        return tags_;
    }
    return block_type_->getTags();
}

const BlockComponentDirectData &Block::getDirectData() const
{
    return direct_data_;
}
