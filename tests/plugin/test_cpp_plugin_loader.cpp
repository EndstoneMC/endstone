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

#include <filesystem>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/detail/logger_factory.h"
#include "endstone/detail/plugin/cpp_plugin_loader.h"
#include "endstone/server.h"

namespace fs = std::filesystem;

class MockServer : public endstone::Server {
public:
    MOCK_METHOD(endstone::Logger &, getLogger, (), (const, override));
    MOCK_METHOD(endstone::PluginManager &, getPluginManager, (), (const, override));
    MOCK_METHOD(endstone::PluginCommand *, getPluginCommand, (std::string name), (const, override));
    MOCK_METHOD(endstone::CommandSender &, getCommandSender, (), (const, override));
    MOCK_METHOD(std::vector<endstone::Level *>, getLevels, (), (const, override));
    MOCK_METHOD(endstone::Level *, getLevel, (std::string name), (const, override));
    MOCK_METHOD(endstone::Player *, getPlayer, (endstone::UUID id), (const, override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(std::string, getVersion, (), (const, override));
    MOCK_METHOD(std::string, getMinecraftVersion, (), (const, override));
    MOCK_METHOD(bool, isPrimaryThread, (), (const, override));
    MockServer()
    {
        ON_CALL(*this, getLogger())
            .WillByDefault(testing::ReturnRef(endstone::detail::LoggerFactory::getLogger("Test")));
    }
};

class CppPluginLoaderTest : public ::testing::Test {
protected:
    // Set Up
    void SetUp() override
    {
        mock_server_ = std::make_unique<MockServer>();
        loader_ = std::make_unique<endstone::detail::CppPluginLoader>(*mock_server_);
        plugin_dir_ = std::filesystem::current_path() / "plugins";
    }

    // Tear Down
    void TearDown() override
    {
        mock_server_.reset();
        loader_.reset();
    }

    std::unique_ptr<MockServer> mock_server_;
    std::unique_ptr<endstone::detail::CppPluginLoader> loader_;
    fs::path plugin_dir_;
};

TEST_F(CppPluginLoaderTest, TestLoadPlugin)
{
    EXPECT_CALL(*mock_server_, getLogger()).Times(2);

    auto nonexistent_path = plugin_dir_ / "nonexistent_file";
    ASSERT_EQ(nullptr, loader_->loadPlugin(nonexistent_path.string()));

#ifdef _WIN32
    auto plugin_path = plugin_dir_ / "test_plugin.dll";
#elif __linux__
    auto plugin_path = plugin_dir_ / "libtest_plugin.so";
#endif
    ASSERT_NE(nullptr, loader_->loadPlugin(plugin_path.string()));
}

TEST_F(CppPluginLoaderTest, TestLoadPluginsFromDirectory)
{
    EXPECT_CALL(*mock_server_, getLogger()).Times(2);
    auto plugins = loader_->loadPlugins(plugin_dir_.string());
    ASSERT_EQ(1, plugins.size());
    ASSERT_EQ(plugins[0]->getName(), "TestPlugin");
    ASSERT_EQ(plugins[0]->getDescription().getVersion(), "1.0.0");
}

TEST_F(CppPluginLoaderTest, TestGetPluginFileFilters)
{
    auto filters = loader_->getPluginFileFilters();
    ASSERT_EQ(1, filters.size());  // Expects one filter pattern
#ifdef _WIN32
    ASSERT_EQ("\\.dll$", filters[0]);  // Expects .dll for windows
#elif __linux__
    ASSERT_EQ("\\.so$", filters[0]);  // Expects .so for linux
#endif
}
