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

#include "bedrock/world/actor/actor_definition_identifier.h"

const std::string &ActorDefinitionIdentifier::getNamespace() const
{
    return namespace_;
}

const std::string &ActorDefinitionIdentifier::getIdentifier() const
{
    return identifier_;
}

const std::string &ActorDefinitionIdentifier::getInitEvent() const
{
    return init_event_;
}

const std::string &ActorDefinitionIdentifier::getCanonicalName() const
{
    return canonical_name_.getString();
}

const HashedString &ActorDefinitionIdentifier::getCanonicalHash() const
{
    return canonical_name_;
}

const std::string &ActorDefinitionIdentifier::getFullName() const
{
    return full_name_;
}

bool ActorDefinitionIdentifier::isVanilla() const
{
    return namespace_ == "minecraft";
}
