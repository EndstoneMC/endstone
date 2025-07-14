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

#include "bedrock/game_version.h"
#include "bedrock/world/level/storage/experiment_storage.h"
#include "bedrock/world/level/storage/storage_version.h"
#include "bedrock/world/level/storage/world_template_level_data.h"

struct AdventureSettings {
    bool no_pvm;
    bool no_mvp;
    bool immutable_world;
    bool show_name_tags;
    bool auto_jump;
};
static_assert(sizeof(AdventureSettings) == 5);

class LevelData {
public:
    explicit LevelData(const bool);
    LevelData(const CompoundTag &);

    [[nodiscard]] int getNetworkVersion() const
    {
        return network_version_;
    }

private:
    AdventureSettings adventure_settings_;
    WorldTemplateLevelData world_template_level_data_;
    GameRules game_rules_;
    ExperimentStorage experiments_;
    Abilities default_abilities_;
    PermissionsHandler default_permissions_;
    std::string level_name_;
    StorageVersion storage_version_;
    GameVersion min_compatible_client_version_;
    int network_version_;
    //...
};
