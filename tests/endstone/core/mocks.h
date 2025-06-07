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

#include <gmock/gmock.h>

#include "endstone/server.h"

class MockServer : public endstone::Server {
public:
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(std::string, getVersion, (), (const, override));
    MOCK_METHOD(std::string, getMinecraftVersion, (), (const, override));
    MOCK_METHOD(endstone::Logger &, getLogger, (), (const, override));
    MOCK_METHOD(endstone::Language &, getLanguage, (), (const, override));
    MOCK_METHOD(endstone::PluginManager &, getPluginManager, (), (const, override));
    MOCK_METHOD(endstone::PluginCommand *, getPluginCommand, (std::string), (const, override));
    MOCK_METHOD(endstone::ConsoleCommandSender &, getCommandSender, (), (const, override));
    MOCK_METHOD(bool, dispatchCommand, (endstone::CommandSender &, std::string), (const, override));
    MOCK_METHOD(endstone::Scheduler &, getScheduler, (), (const, override));
    MOCK_METHOD(endstone::ServiceManager &, getServiceManager, (), (const, override));
    MOCK_METHOD(endstone::Registry<endstone::Enchantment> &, getEnchantmentRegistry, (), (const, override));
    MOCK_METHOD(endstone::Level *, getLevel, (), (const, override));
    MOCK_METHOD(std::vector<endstone::Player *>, getOnlinePlayers, (), (const, override));
    MOCK_METHOD(int, getMaxPlayers, (), (const, override));
    MOCK_METHOD(endstone::Result<void>, setMaxPlayers, (int), (override));
    MOCK_METHOD(endstone::Player *, getPlayer, (endstone::UUID), (const, override));
    MOCK_METHOD(endstone::Player *, getPlayer, (std::string), (const, override));
    MOCK_METHOD(bool, getOnlineMode, (), (const, override));
    MOCK_METHOD(void, shutdown, (), (override));
    MOCK_METHOD(void, reload, (), (override));
    MOCK_METHOD(void, reloadData, (), (override));
    MOCK_METHOD(void, broadcast, (const endstone::Message &, const std::string &), (const, override));
    MOCK_METHOD(void, broadcastMessage, (const endstone::Message &), (const, override));
    MOCK_METHOD(bool, isPrimaryThread, (), (const, override));
    MOCK_METHOD(endstone::ItemFactory &, getItemFactory, (), (const, override));
    MOCK_METHOD(endstone::Scoreboard *, getScoreboard, (), (const, override));
    MOCK_METHOD(std::shared_ptr<endstone::Scoreboard>, createScoreboard, (), (override));
    MOCK_METHOD(float, getCurrentMillisecondsPerTick, (), (override));
    MOCK_METHOD(float, getAverageMillisecondsPerTick, (), (override));
    MOCK_METHOD(float, getCurrentTicksPerSecond, (), (override));
    MOCK_METHOD(float, getAverageTicksPerSecond, (), (override));
    MOCK_METHOD(float, getCurrentTickUsage, (), (override));
    MOCK_METHOD(float, getAverageTickUsage, (), (override));
    MOCK_METHOD(std::chrono::system_clock::time_point, getStartTime, (), (override));
    MOCK_METHOD(std::unique_ptr<endstone::BossBar>, createBossBar,
                (std::string, endstone::BarColor, endstone::BarStyle), (const, override));
    MOCK_METHOD(std::unique_ptr<endstone::BossBar>, createBossBar,
                (std::string, endstone::BarColor, endstone::BarStyle, std::vector<endstone::BarFlag>),
                (const, override));
    MOCK_METHOD(endstone::Result<std::unique_ptr<endstone::BlockData>>, createBlockData, (std::string),
                (const, override));
    MOCK_METHOD(endstone::Result<std::unique_ptr<endstone::BlockData>>, createBlockData,
                (std::string, endstone::BlockStates), (const, override));
    MOCK_METHOD(endstone::PlayerBanList &, getBanList, (), (const, override));
    MOCK_METHOD(endstone::IpBanList &, getIpBanList, (), (const, override));
};

class MockPlugin : public endstone::Plugin {
public:
    MOCK_METHOD(const endstone::PluginDescription &, getDescription, (), (const, override));

    void setEnabled(const bool enabled)
    {
        Plugin::setEnabled(enabled);
    }
};
