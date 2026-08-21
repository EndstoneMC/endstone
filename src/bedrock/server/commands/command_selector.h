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
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/forward.h"
#include "bedrock/server/commands/command.h"
#include "bedrock/world/actor/actor_definition_identifier.h"
#include "bedrock/world/actor/selectors/actor_selector.h"

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

    using FilterFunc = std::function<bool(const CommandOrigin &, const Actor &)>;

    [[nodiscard]] CommandSelectionType getType() const { return type_; }
    [[nodiscard]] CommandSelectionOrder getOrder() const { return order_; }
    [[nodiscard]] std::size_t getResultCount() const { return count_; }
    [[nodiscard]] const CommandPosition &getPosition() const { return position_; }
    [[nodiscard]] bool isExplicitIdSelector() const { return is_explicit_id_selector_; }

protected:
    explicit CommandSelectorBase(bool force_player) : force_player_(force_player) {}

    [[nodiscard]] CommandResultVector newResults(const CommandOrigin &origin) const;

private:
    int version_{0};                                                         // +0
    CommandSelectionType type_{CommandSelectionType::Self};                  // +4
    CommandSelectionOrder order_{CommandSelectionOrder::Sorted};             // +8
    std::vector<InvertableFilter<std::string>> name_filters_;                // +16
    std::vector<InvertableFilter<ActorDefinitionIdentifier>> type_filters_;  // +40
    std::vector<InvertableFilter<HashedString>> family_filters_;             // +64
    std::vector<InvertableFilter<std::string>> tag_filters_;                 // +88
    std::vector<FilterFunc> filter_chain_;                                   // +112
    CommandPosition position_{};                                             // +136
    Vec3 box_deltas_{};                                                      // +152
    float radius_min_sqr_{0.0F};                                             // +164
    float radius_max_sqr_{std::numeric_limits<float>::max()};                // +168
    std::size_t count_{Unlimited};                                           // +176
    bool include_dead_players_{false};                                       // +184
    bool is_position_bound_{false};                                          // +185
    bool distance_filtered_{false};                                          // +186
    bool position_filtered_{false};                                          // +187
    bool count_filtered_{false};                                             // +188
    bool have_deltas_{false};                                                // +189
    bool force_player_{false};                                               // +190
    bool exclude_agents_{false};                                             // +191
    bool is_explicit_id_selector_{false};                                    // +192
    bool force_dimension_filtering_{false};                                  // +193
    bool is_name_filters_only_in_chain_{true};                               // +194
};
BEDROCK_STATIC_ASSERT_SIZE(CommandSelectorBase, 200, 200);

template <typename T>
class CommandSelector : public CommandSelectorBase {
public:
    CommandSelector() : CommandSelectorBase(std::is_same_v<T, ::Player>) {}

    using CommandSelectorBase::newResults;
};

using ActorSelector = CommandSelector<Actor>;
using PlayerSelector = CommandSelector<Player>;

/**
 * The parse rule the command registry uses for a target selector.
 *
 * The actor and the player selector share one rule; which of the two a parameter yields is decided
 * by the storage it is parsed into, not by the rule.
 */
const CommandRegistry::ParamParseRule &getCommandSelectorParseRule();
