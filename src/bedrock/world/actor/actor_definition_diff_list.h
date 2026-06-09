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

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <entt/container/dense_map.hpp>

#include "bedrock/world/actor/diff_list_pair.h"

class ActorDefinitionGroup;
class ActorDefinitionDescriptor;
class IDefinitionInstance;

class DefinitionInstanceGroup {
public:
    using DenseMap = entt::dense_map<unsigned short, std::shared_ptr<IDefinitionInstance>>;

    void add(std::shared_ptr<IDefinitionInstance> const &definition);
    void clear();
    void combine(DefinitionInstanceGroup const &other);
    bool contains(DefinitionInstanceGroup const &other) const;
    bool overlaps(DefinitionInstanceGroup const &other) const;
    void subtract(DefinitionInstanceGroup const &other);
    [[nodiscard]] DenseMap const &getMap() const { return map_; }

private:
    DenseMap map_;
};

class ActorDefinitionDiffList {

    ActorDefinitionGroup &definitions_;                               // +0
    std::vector<DiffListPair> definition_stack_;                      // +8
    std::unique_ptr<ActorDefinitionDescriptor> changed_description_;  // +32
    bool changed_;                                                    // +40
    DefinitionInstanceGroup added_definition_group_;                  // +48
    DefinitionInstanceGroup removed_definition_group_;                // +104*
    std::unordered_map<std::uint64_t, std::variant<int, float, bool, std::uint64_t>>
        pending_property_changes_;                               // +160*
    bool locked_changes_;                                        // +224*
    std::vector<std::pair<bool, std::string>> pending_changes_;  // +232*
    std::unordered_map<std::uint64_t, std::variant<int, float, bool, std::uint64_t>>
        deferred_property_changes_;  // +256*

public:
    ActorDefinitionDiffList() = delete;
    [[nodiscard]] std::vector<DiffListPair> const &getDefinitionStack() const { return definition_stack_; }
};
