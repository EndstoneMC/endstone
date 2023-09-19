// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <memory>

#include "bedrock/internal.h"
#include "bedrock/types.h"

class DedicatedServer {
public:
    enum class StartResult;

    BEDROCK_API DedicatedServer::StartResult runDedicatedServerLoop(
        Core::FilePathManager &file_path_manager, class PropertiesSettings &properties_settings,
        class LevelSettings &level_settings, class AllowListFile &allow_list_file,
        class std::unique_ptr<class PermissionsFile> &permissions_file);

private:
    BEDROCK_API void initializeLogging();
};
