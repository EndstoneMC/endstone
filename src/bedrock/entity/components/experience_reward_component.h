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

#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/util/molang/expression_node.h"

class Actor;
class ExperienceRewardComponent;

class ExperienceRewardDefinition {
public:
    void initialize(EntityContext &entity, ExperienceRewardComponent &reward) const;
    void addBredExpressionNode(const ExpressionNode &node);
    void addDeathExpressionNode(const ExpressionNode &node);
    std::vector<ExpressionNode> on_bred;
    std::vector<ExpressionNode> on_death;
};
static_assert(sizeof(ExperienceRewardDefinition) == 48);

class ExperienceRewardComponent {
public:
    int getOnBredExperience(Actor &) const;
    int getOnDeathExperience(Actor &) const;
    bool getIsExperienceDropEnabled() const;
    void setIsExperienceDropEnabled(const bool);

private:
    friend class ExperienceRewardDefinition;
    std::vector<ExpressionNode> on_bred_;
    std::vector<ExpressionNode> on_death_;
    bool is_experience_drop_enabled_ = true;
};
static_assert(sizeof(ExperienceRewardComponent) == 56);
