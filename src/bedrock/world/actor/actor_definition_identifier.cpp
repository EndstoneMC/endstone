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

ActorDefinitionIdentifier::ActorDefinitionIdentifier(const std::string &full_name)
{
    initialize(full_name);
}

void ActorDefinitionIdentifier::initialize(const std::string &full_name)
{
    _extractIdentifier(full_name, *this);
    _initialize();
}

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

void ActorDefinitionIdentifier::_initialize()
{
    if (canonical_name_.empty()) {
        canonical_name_ = namespace_ + NAMESPACE_SEPARATOR + identifier_;
    }
    full_name_ = canonical_name_.getString() + EVENT_BEGIN + init_event_ + EVENT_END;
}

void ActorDefinitionIdentifier::_extractIdentifier(const std::string &name, ActorDefinitionIdentifier &id)
{
    std::string_view sv{name};
    std::string_view ns;
    std::string_view identifier;
    std::string_view init_event;

    if (const auto colon = sv.find_first_of(NAMESPACE_SEPARATOR); colon != std::string_view::npos) {
        ns = sv.substr(0, colon);
        sv = sv.substr(colon + 1);
    }
    if (ns.empty()) {
        ns = "minecraft";
    }

    if (const auto lt = sv.find_first_of(EVENT_BEGIN); lt != std::string_view::npos) {
        if (const auto gt = sv.find_first_of(EVENT_END, lt + 1); gt != std::string_view::npos && gt > lt + 1) {
            init_event = sv.substr(lt + 1, gt - lt - 1);
        }
        identifier = sv.substr(0, lt);
    }

    id.namespace_ = ns;
    id.identifier_ = identifier;
    id.init_event_ = init_event;
}
