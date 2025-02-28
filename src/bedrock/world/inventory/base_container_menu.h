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

#include "bedrock/forward.h"
#include "bedrock/world/container.h"
#include "bedrock/world/containers/managers/container_manager.h"

class BaseContainerMenu : public ContainerContentChangeListener, public IContainerManager {
public:
    static const int INVENTORY_TRACKER_CLEANUP_INTERVAL = 100;
    BaseContainerMenu(Player &, ContainerType);
    ~BaseContainerMenu() override = default;
    void containerContentChanged(int) override;

protected:
    Player &player_;
    std::vector<ItemStack> last_slots_;
    std::unique_ptr<PlayerContainerRefresher> container_refresher_;
    std::shared_ptr<DynamicContainerTracker> dynamic_container_tracker_;

private:
    ContainerID container_id_;
    ContainerType container_type_;
    int ticks_since_last_cleanup_;
    Bedrock::PubSub::PublisherPtr<void(ContainerType), Bedrock::PubSub::ThreadModel::SingleThreaded>
        container_type_subscriber_;
};
static_assert(sizeof(BaseContainerMenu) == 88);
