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

#include <string>

#include <entt/entt.hpp>

#include "bedrock/entity/entity_id.h"
#include "bedrock/gamerefs/enable_get_weak_ref.h"

template <typename>
class WeakRef;

class EntityRegistry : public EnableGetWeakRef<EntityRegistry>, public std::enable_shared_from_this<EntityRegistry> {
public:
    WeakRef<EntityRegistry> getWeakRef();

protected:
    friend class EntityContext;

    // NOTE: uncomment the next line if you are running endstone on preview instead of release
    // bool is_viewing_;
    std::string debug_name_;
    EnTTRegistry registry_;
    std::uint32_t id_;
};
