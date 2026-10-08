// Copyright (c) 2026, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"
#include "mocks.h"

using namespace endstone;
using namespace endstone::core;

class PluginManagerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ON_CALL(server_, isPrimaryThread()).WillByDefault(testing::Return(true));
        plugin_.setEnabled(true);
    }

    testing::NiceMock<MockServer> server_;
    MockPlugin plugin_;
    EndstonePluginManager manager_{server_};
};

TEST_F(PluginManagerTest, IsEventRegistered)
{
    EXPECT_FALSE(manager_.isEventRegistered<PacketSendEvent>());

    manager_.registerEvent(PacketSendEvent::NAME, [](Event &) {}, EventPriority::Normal, plugin_, false);
    EXPECT_TRUE(manager_.isEventRegistered<PacketSendEvent>());
    EXPECT_FALSE(manager_.isEventRegistered<PacketReceiveEvent>());
}

TEST_F(PluginManagerTest, IsEventRegisteredAfterCallWithoutHandlers)
{
    PacketReceiveEvent e{nullptr, 0, {}, SocketAddress{}, 0};
    manager_.callEvent(e);
    EXPECT_FALSE(manager_.isEventRegistered<PacketReceiveEvent>());
}
