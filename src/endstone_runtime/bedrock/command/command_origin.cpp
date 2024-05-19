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

#include "bedrock/command/command_origin.h"

#include "bedrock/command/command_origin_data.h"
#include "endstone/detail/hook.h"

std::unique_ptr<CommandOrigin> CommandOrigin::fromCommandOriginData(const CommandOriginData &data,
                                                                    const Bedrock::NonOwnerPointer<ILevel> &level,
                                                                    const NetworkIdentifier &network_id,
                                                                    SubClientId sub_client_id)
{
    std::unique_ptr<CommandOrigin> result;
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&CommandOrigin::fromCommandOriginData, result, data, level, network_id,
                                    sub_client_id);
    return result;
}
