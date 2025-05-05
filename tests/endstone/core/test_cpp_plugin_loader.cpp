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

#include "endstone/core/logger_factory.h"
#include "endstone/core/plugin/cpp_plugin_loader.h"
#include "mocks.h"

namespace fs = std::filesystem;

class CppPluginLoaderTest : public ::testing::Test {
protected:
    // Set Up
    void SetUp() override
    {
        ON_CALL(server_, getLogger())
            .WillByDefault(testing::ReturnRef(endstone::core::LoggerFactory::getLogger("Test")));
        loader_ = std::make_unique<endstone::core::CppPluginLoader>(server_);
        plugin_dir_ = std::filesystem::current_path() / "plugins";
    }

    // Tear Down
    void TearDown() override
    {
        loader_.reset();
    }

    testing::NiceMock<MockServer> server_;
    std::unique_ptr<endstone::core::CppPluginLoader> loader_;
    fs::path plugin_dir_;
};

TEST_F(CppPluginLoaderTest, TestLoadPlugin)
{
    auto nonexistent_path = plugin_dir_ / "nonexistent_file";
    ASSERT_EQ(nullptr, loader_->loadPlugin(nonexistent_path.string()));

#ifdef _WIN32
    auto plugin_path = plugin_dir_ / "endstone_test_plugin.dll";
#elif __linux__
    auto plugin_path = plugin_dir_ / "endstone_test_plugin.so";
#endif
    ASSERT_NE(nullptr, loader_->loadPlugin(plugin_path.string()));
}

TEST_F(CppPluginLoaderTest, TestLoadPluginsFromDirectory)
{
    auto plugins = loader_->loadPlugins(plugin_dir_.string());
    ASSERT_EQ(1, plugins.size());
    ASSERT_EQ(plugins[0]->getName(), "test");
    ASSERT_EQ(plugins[0]->getDescription().getPrefix(), "TestPlugin");
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
