//
// Created by Vincent on 23/08/2023.
//

#include "endstone/plugin/cpp/cpp_plugin.h"

#include <gtest/gtest.h>

class MockCppPlugin : public CppPlugin {
public:
    const PluginDescription &getDescription() const override
    {
        static CppPluginDescription desc("TestPlugin", "1.0");
        return desc;
    }

    friend class CppPluginTest_SetEnabledTest_Test;
};

TEST(CppPluginTest, IsEnabledTest)
{
    MockCppPlugin plugin;
    EXPECT_EQ(plugin.isEnabled(), false);
}

TEST(CppPluginTest, PluginLoaderTest)
{
    MockCppPlugin plugin;
    EXPECT_EQ(plugin.getPluginLoader(), nullptr);
}

TEST(CppPluginTest, SetEnabledTest)
{
    MockCppPlugin plugin;
    plugin.setEnabled(true);
    EXPECT_EQ(plugin.isEnabled(), true);
    plugin.setEnabled(false);
    EXPECT_EQ(plugin.isEnabled(), false);
}
