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

#include "bedrock/entity/utilities/synched_actor_data_access.h"

#include "bedrock/entity/components/actor_data_dirty_flags_component.h"
#include "bedrock/entity/components/actor_data_flag_component.h"

bool SynchedActorDataAccess::getActorFlag(const EntityContext &entity, ActorFlags flag)
{
    if (auto *data_flag = entity.tryGetComponent<ActorDataFlagComponent>()) {
        return data_flag->getStatusFlag(flag);
    }
    return false;
}

void SynchedActorDataAccess::setActorFlag(EntityContext &entity, ActorFlags flag, bool value)
{
    auto &dirty_flags = entity.getOrAddComponent<ActorDataDirtyFlagsComponent>();
    auto &data_flag = entity.getOrAddComponent<ActorDataFlagComponent>();

    if (data_flag.getStatusFlag(flag) != value) {
        data_flag.setStatusFlag(flag, value);
        if (flag < ActorFlags::LAYING_DOWN) {
            dirty_flags.dirty_flags.set(static_cast<size_t>(ActorDataIDs::RESERVED_0), true);
        }
        else {
            dirty_flags.dirty_flags.set(static_cast<size_t>(ActorDataIDs::RESERVED_092), true);
        }
    }
}
