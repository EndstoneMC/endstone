//
// Created by Vincent on 23/08/2023.
//

#include "endstone/plugin/cpp/cpp_plugin_description.h"

#include <gtest/gtest.h>

class CppPluginDescriptionTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CppPluginDescriptionTest, Constructor_ValidName_SetsName)
{
    CppPluginDescription plugin("TestPlugin", "1.0");
    EXPECT_EQ("TestPlugin", plugin.getName());
    EXPECT_EQ("1.0", plugin.getVersion());
}

TEST_F(CppPluginDescriptionTest, Constructor_InvalidName_Throws)
{
    EXPECT_THROW(CppPluginDescription plugin("Test#Plugin", "1.0"), std::invalid_argument);
}

TEST_F(CppPluginDescriptionTest, Constructor_ReplacesSpacesWithUnderscores)
{
    CppPluginDescription plugin("Test Plugin", "1.0");
    EXPECT_EQ("Test_Plugin", plugin.getName());
}

TEST_F(CppPluginDescriptionTest, GetName)
{
    CppPluginDescription plugin("MyPlugin", "1.0");
    EXPECT_EQ("MyPlugin", plugin.getName());
}

TEST_F(CppPluginDescriptionTest, GetVersion)
{
    CppPluginDescription plugin("MyPlugin", "2.5");
    EXPECT_EQ("2.5", plugin.getVersion());
}

TEST_F(CppPluginDescriptionTest, GetDescription_ReturnsNullopt)
{
    CppPluginDescription plugin("MyPlugin", "1.0");
    EXPECT_EQ(std::nullopt, plugin.getDescription());
}

TEST_F(CppPluginDescriptionTest, GetAuthors_ReturnsNullopt)
{
    CppPluginDescription plugin("MyPlugin", "1.0");
    EXPECT_EQ(std::nullopt, plugin.getAuthors());
}

TEST_F(CppPluginDescriptionTest, GetPrefix_ReturnsNullopt)
{
    CppPluginDescription plugin("MyPlugin", "1.0");
    EXPECT_EQ(std::nullopt, plugin.getPrefix());
}

TEST_F(CppPluginDescriptionTest, GetFullName)
{
    CppPluginDescription plugin("MyPlugin", "1.2.3");
    EXPECT_EQ("MyPlugin v1.2.3", plugin.getFullName());
}
