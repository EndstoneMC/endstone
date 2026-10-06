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

#include <memory>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/event_handler.h"
#include "endstone/event/handler_list.h"
#include "mocks.h"

namespace {

class TestEvent : public endstone::Cancellable<endstone::Event> {
public:
    using endstone::Cancellable<endstone::Event>::Cancellable;
    ENDSTONE_EVENT(TestEvent)
};

}  // namespace

TEST(EventHandlerTest, PreservesOrderingCancellationAndDisabledListeners)
{
    using ::testing::Return;

    MockServer server;
    EXPECT_CALL(server, isPrimaryThread()).WillRepeatedly(Return(true));

    endstone::core::EndstonePluginManager manager(server);
    MockPlugin first_plugin;
    MockPlugin second_plugin;
    first_plugin.setEnabled(true);
    second_plugin.setEnabled(true);

    std::vector<int> calls;
    manager.registerEvent(
        TestEvent::NAME,
        [&](endstone::Event &event) {
            calls.push_back(1);
            static_cast<TestEvent &>(event).cancel();
        },
        endstone::EventPriority::Low, first_plugin, false);
    manager.registerEvent(TestEvent::NAME, [&](endstone::Event &) { calls.push_back(2); },
                          endstone::EventPriority::Normal, second_plugin, true);
    manager.registerEvent(TestEvent::NAME, [&](endstone::Event &) { calls.push_back(3); },
                          endstone::EventPriority::Highest, second_plugin, false);

    TestEvent event;
    manager.callEvent(event);
    EXPECT_TRUE(event.isCancelled());
    EXPECT_THAT(calls, ::testing::ElementsAre(1, 3));

    calls.clear();
    second_plugin.setEnabled(false);

    TestEvent next_event;
    manager.callEvent(next_event);
    EXPECT_THAT(calls, ::testing::ElementsAre(1));
}

TEST(EventHandlerTest, UnregistersPluginListeners)
{
    MockPlugin plugin;
    endstone::HandlerList handlers(TestEvent::NAME);

    auto *handler = handlers.registerHandler(std::make_unique<endstone::EventHandler>(
        TestEvent::NAME, [](endstone::Event &) {}, endstone::EventPriority::Normal, plugin, false));

    ASSERT_NE(handler, nullptr);
    ASSERT_EQ(handlers.getHandlers().size(), 1);

    handlers.unregister(plugin);
    EXPECT_TRUE(handlers.getHandlers().empty());
}
