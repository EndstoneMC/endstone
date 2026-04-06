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

#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/gamerefs/owner_ptr.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/actor_definition_identifier.h"
#include "bedrock/world/actor/actor_registry.h"

class Level;
using ActorDataMap = std::unordered_map<std::string, ActorFactoryData>;

class IEntityInitializer {
public:
    virtual ~IEntityInitializer();
    virtual void entityInitializer(std::unique_ptr<Actor>, EntityContext &, const EntityContext &) = 0;
};

class ActorFactory {
public:
    ActorFactory(Bedrock::NotNullNonOwnerPtr<Level> level);
    virtual ~ActorFactory();
    OwnerPtr<EntityContext> createSpawnedActor(const ActorDefinitionIdentifier &identifier, Actor *spawner,
                                               const Vec3 &position, const Vec2 &rotation);

    [[nodiscard]] std::vector<std::string> generateActorIdentifierList() const;

private:
    Bedrock::NotNullNonOwnerPtr<Level> level_;
    std::shared_ptr<IEntityInitializer> entity_initializer_;
    Bedrock::NonOwnerPointer<ActorDefinitionGroup> definitions_;
    ActorDataMap factory_function_;
    // ...
};
