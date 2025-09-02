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

#include "bedrock/world/level/block/block_type.h"

#include "bedrock/symbol.h"
#include "bedrock/world/item/registry/item_registry.h"

bool BlockType::hasProperty(BlockProperty property) const
{
    return (static_cast<std::underlying_type_t<BlockProperty>>(property) &
            static_cast<std::underlying_type_t<BlockProperty>>(properties_)) != 0;
}

const Block *BlockType::tryGetStateFromLegacyData(DataID data) const
{
    return BEDROCK_CALL(&BlockType::tryGetStateFromLegacyData, this, data);
}

bool BlockType::hasState(const BlockState &block_state) const
{
    if (states_.contains(block_state.getID())) {
        return true;
    }
    for (const auto &altered_state : altered_state_collections_) {
        if (altered_state->getBlockState().getID() == block_state.getID()) {
            return true;
        }
    }
    return false;
}

bool BlockType::hasState(const HashedString &name) const
{
    auto *state = BlockStateRegistry::get().getState(name);
    if (!state) {
        return false;
    }
    return hasState(*state);
}

bool BlockType::requiresCorrectToolForDrops() const
{
    return requires_correct_tool_for_drops_;
}

bool BlockType::isSolid() const
{
    return solid_;
}

float BlockType::getThickness() const
{
    return thickness_;
}

float BlockType::getTranslucency() const
{
    return translucency_;
}

const std::vector<HashedString> &BlockType::getTags() const
{
    return tags_;
}

const Material &BlockType::getMaterial() const
{
    return material_;
}

const std::string &BlockType::getDescriptionId() const
{
    return description_id;
}

const std::string &BlockType::getRawNameId() const
{
    return name_info_.raw_name.getString();
}

const std::string &BlockType::getNamespace() const
{
    return name_info_.namespace_name;
}

const HashedString &BlockType::getName() const
{
    return name_info_.full_name;
}

bool BlockType::anyOf(const gsl::span<const std::reference_wrapper<const HashedString>> &block_type_ids) const
{
    return std::ranges::any_of(block_type_ids,
                               [this](const auto &block_type_id) { return block_type_id.get() == getName(); });
}

const Block &BlockType::getDefaultState() const
{
    return *default_state_;
}

const BaseGameVersion &BlockType::getRequiredBaseGameVersion() const
{
    return min_required_game_version_;
}

std::int16_t BlockType::getBlockItemId() const
{
    const auto value = id_.value;
    if (value < ItemRegistry::START_ITEM_ID) {
        return static_cast<std::int16_t>(value);
    }
    return static_cast<std::int16_t>((ItemRegistry::START_ITEM_ID - 1u) - value);
}

TintMethod BlockType::getTintMethod() const
{
    return tint_method_;
}

void BlockType::forEachBlockPermutation(std::function<bool(Block const &)> callback) const
{
    for (const auto &block_permutation : block_permutations_) {
        if (block_permutation) {
            (void)callback(*block_permutation);
        }
    }
}

std::optional<int> BlockType::_tryLookupAlteredStateCollection(size_t id, DataID data) const
{
    if (altered_state_collections_.empty()) {
        return std::nullopt;
    }
    for (const auto &altered_state : altered_state_collections_) {
        if (altered_state->getBlockState().getID() == id) {
            return altered_state->getState(*this, data);
        }
    }
    return std::nullopt;
}

const Block *BlockType::_trySetStateFromAlteredStateCollection(size_t id, int val, DataID data) const
{
    if (altered_state_collections_.empty()) {
        return nullptr;
    }
    for (const auto &altered_state : altered_state_collections_) {
        if (altered_state->getBlockState().getID() == id) {
            return altered_state->setState(*this, data, val);
        }
    }
    return nullptr;
}
