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

#pragma once

#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/forward.h"
#include "bedrock/server/commands/command.h"
#include "bedrock/world/actor/actor_definition_identifier.h"
#include "bedrock/world/actor/selectors/actor_selector.h"

namespace endstone::core {
class BrigadierCommandAdapter;
}  // namespace endstone::core

enum class CommandSelectionType : int {
    Self = 0,
    Entities = 1,
    Players = 2,
    DefaultPlayers = 3,
    OwnedAgent = 4,
    Agents = 5,
    Initiator = 6,
};

enum class CommandSelectionOrder : int {
    Sorted = 0,
    InverseSorted = 1,
    Random = 2,
};

using CommandResultVector = std::shared_ptr<std::vector<Actor *>>;

class CommandSelectorBase {
public:
    static constexpr std::size_t Unlimited = 4294967295;
    // Endstone begins
    CommandSelectorBase(const CommandSelectorBase &) = delete;
    CommandSelectorBase(CommandSelectorBase &&) = delete;
    CommandSelectorBase &operator=(const CommandSelectorBase &) = delete;
    CommandSelectorBase &operator=(CommandSelectorBase &&) = delete;
    // Endstone ends

protected:
    explicit CommandSelectorBase(bool force_player) : force_player_(force_player) {}
    [[nodiscard]] CommandResultVector newResults(const CommandOrigin &origin) const;

private:
    // Endstone
    friend class endstone::core::BrigadierCommandAdapter;

    int version_{0};
    CommandSelectionType type_{CommandSelectionType::Self};
    CommandSelectionOrder order_{CommandSelectionOrder::Sorted};
    std::vector<InvertableFilter<std::string>> name_filters_;
    std::vector<InvertableFilter<ActorDefinitionIdentifier>> type_filters_;
    std::vector<InvertableFilter<HashedString>> family_filters_;
    std::vector<InvertableFilter<std::string>> tag_filters_;
    std::vector<std::function<bool(const CommandOrigin &, const Actor &)>> filter_chain_;
    CommandPosition position_;
    Vec3 box_deltas_{0.0F, 0.0F, 0.0F};
    float radius_min_sqr_{0.0F};
    float radius_max_sqr_{std::numeric_limits<float>::max()};
    std::size_t count_{Unlimited};
    bool include_dead_players_{false};
    bool is_position_bound_{false};
    bool distance_filtered_{false};
    bool position_filtered_{false};
    bool count_filtered_{false};
    bool have_deltas_{false};
    bool force_player_;
    bool exclude_agents_{false};
    bool is_explicit_id_selector_{false};
    bool force_dimension_filtering_{false};
    bool is_code_builder_enabled_{false};
};
BEDROCK_STATIC_ASSERT_SIZE(CommandSelectorBase, 200, 200);

template <typename T>
class CommandSelectorResults {
private:
    template <typename>
    friend class CommandSelector;
    friend class endstone::core::BrigadierCommandAdapter;  // Endstone

    explicit CommandSelectorResults(CommandResultVector targets) : targets_(std::move(targets)) {}

    CommandResultVector targets_;
};
BEDROCK_STATIC_ASSERT_SIZE(CommandSelectorResults<Actor>, 16, 16);
BEDROCK_STATIC_ASSERT_SIZE(CommandSelectorResults<Player>, 16, 16);

template <typename T>
class CommandSelector : public CommandSelectorBase {
public:
    CommandSelector() : CommandSelectorBase(std::is_same_v<T, Player>) {}

    [[nodiscard]] CommandSelectorResults<T> results(const CommandOrigin &origin) const
    {
        return CommandSelectorResults<T>(newResults(origin));
    }
};
BEDROCK_STATIC_ASSERT_SIZE(CommandSelector<Actor>, 200, 200);
BEDROCK_STATIC_ASSERT_SIZE(CommandSelector<Player>, 200, 200);

using ActorSelector = CommandSelector<Actor>;
using PlayerSelector = CommandSelector<Player>;
