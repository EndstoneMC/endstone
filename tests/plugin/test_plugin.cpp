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

#include "endstone/plugin/plugin.h"

class TestPlugin : public endstone::Plugin {
public:
    void onLoad() override
    {
        getLogger().info("onLoad is called");
    }

    void onEnable() override
    {
        getLogger().info("onEnable is called");
    }

    void onDisable() override
    {
        getLogger().info("onDisable is called");
    }
};

ENDSTONE_PLUGIN("TestPlugin", "1.0.0", TestPlugin)
{
    description = "This is a test plugin";
    website = "https://github.com/EndstoneMC/endstone";
    authors = {"Endstone Developers <hello@endstone.dev>"};
    load = endstone::PluginLoadOrder::PostWorld;
    prefix = "TestPlugin";

    permission("test_plugin.command.test")  //
        .description("Allow users to use the test command");
    permission("test_plugin.command")  //
        .description("Allow users to use all commands provided by this example plugin")
        .children("test_plugin.command.test", true);

    command("test")
        .description("Test command")
        .usages("/test")
        .aliases("tryme")
        .permissions("test_plugin.command.test");
}
