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

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <gsl/pointers>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/gamerefs/weak_ref.h"
#include "bedrock/world/container_id.h"
#include "bedrock/world/containers/managers/container_manager.h"
#include "bedrock/world/containers/managers/container_screen_context.h"
#include "bedrock/world/item/item_stack.h"

class ContainerManagerController;
class ContainerModel;
class DynamicContainerTracker;
class IContainerRegistryAccess;
class Player;
class PlayerContainerRefresher;

class ContainerManagerModel : public IContainerManager {
public:
    ContainerManagerModel(ContainerID container_id, Player &player);
    ~ContainerManagerModel() override = 0;

protected:
    gsl::not_null<std::unique_ptr<PlayerContainerRefresher>> container_refresher_;
    gsl::not_null<std::shared_ptr<DynamicContainerTracker>> dynamic_container_tracker_;
    WeakRef<IContainerRegistryAccess> registry_access_;
    Player &player_;
    std::vector<ItemStack> last_slots_;
    ContainerID container_id_;
    ContainerType container_type_;
    Bedrock::PubSub::PublisherPtr<void(ContainerType), Bedrock::PubSub::ThreadModel::SingleThreaded>
        container_type_subscribers_;
    std::unordered_map<const ContainerManagerController *, std::function<void(ContainerManagerModel &)>>
        inform_controller_of_destruction_callbacks_;
    ContainerScreenContext screen_context_;
    std::unordered_map<std::string, std::shared_ptr<ContainerModel>> containers_;
    Bedrock::PubSub::Publisher<void(), Bedrock::PubSub::ThreadModel::MultiThreaded>
        on_dynamic_container_change_publisher_;
};
BEDROCK_STATIC_ASSERT_SIZE(ContainerManagerModel, 392, 304);
