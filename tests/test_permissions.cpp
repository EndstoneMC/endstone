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
#include <string>
#include <unordered_set>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/endstone.hpp"
#include "mocks.h"

using namespace endstone;
using namespace endstone::core;

namespace {

class FakePermissible : public Permissible {
public:
    [[nodiscard]] PermissionLevel getPermissionLevel() const override { return PermissionLevel::Default; }
    [[nodiscard]] bool isPermissionSet(std::string) const override { return false; }
    [[nodiscard]] bool isPermissionSet(const NotNull<Permission> &) const override { return false; }
    [[nodiscard]] bool hasPermission(std::string) const override { return false; }
    [[nodiscard]] bool hasPermission(const NotNull<Permission> &) const override { return false; }

    NotNull<PermissionAttachment> addAttachment(Plugin &plugin, const std::string &, bool) override
    {
        return addAttachment(plugin);
    }

    NotNull<PermissionAttachment> addAttachment(Plugin &plugin) override
    {
        return std::make_shared<PermissionAttachment>(plugin, shared_from_this());
    }

    bool removeAttachment(const NotNull<PermissionAttachment> &) override { return false; }
    void recalculatePermissions() override { ++recalculated; }

    [[nodiscard]] std::unordered_set<NotNull<PermissionAttachmentInfo>> getEffectivePermissions() const override
    {
        return {};
    }

    [[nodiscard]] const std::type_info &getClassTypeId() const override { return typeid(Permissible); }
    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return target == typeid(Permissible);
    }

    int recalculated = 0;
};

}  // namespace

TEST(PermissionSubscriptionTest, DropsDestroyedPermissible)
{
    testing::NiceMock<MockServer> server;
    EndstonePluginManager manager{server};

    auto permissible = std::make_shared<FakePermissible>();
    manager.subscribeToPermission("endstone.test", permissible);
    EXPECT_EQ(manager.getPermissionSubscriptions("endstone.test").size(), 1);

    permissible.reset();
    EXPECT_TRUE(manager.getPermissionSubscriptions("endstone.test").empty());
    EXPECT_TRUE(manager.getPermissionSubscriptions("endstone.test").empty());
}

TEST(PermissionSubscriptionTest, KeepsLivePermissible)
{
    testing::NiceMock<MockServer> server;
    EndstonePluginManager manager{server};

    auto permissible = std::make_shared<FakePermissible>();
    const auto retained = permissible;
    manager.subscribeToPermission("endstone.test", permissible);

    permissible.reset();
    EXPECT_EQ(manager.getPermissionSubscriptions("endstone.test").size(), 1);
}

TEST(PermissionSubscriptionTest, DropsDestroyedPermissibleFromDefaultPerms)
{
    testing::NiceMock<MockServer> server;
    EndstonePluginManager manager{server};

    auto permissible = std::make_shared<FakePermissible>();
    manager.subscribeToDefaultPerms(PermissionLevel::Operator, permissible);
    EXPECT_EQ(manager.getDefaultPermSubscriptions(PermissionLevel::Operator).size(), 1);

    permissible.reset();
    EXPECT_TRUE(manager.getDefaultPermSubscriptions(PermissionLevel::Operator).empty());
}

TEST(PermissionSubscriptionTest, UnsubscribeRemovesPermissible)
{
    testing::NiceMock<MockServer> server;
    EndstonePluginManager manager{server};

    const auto permissible = std::make_shared<FakePermissible>();
    manager.subscribeToPermission("endstone.test", permissible);
    manager.unsubscribeFromPermission("endstone.test", permissible);
    EXPECT_TRUE(manager.getPermissionSubscriptions("endstone.test").empty());
}

TEST(PermissionAttachmentTest, DoesNotKeepPermissibleAlive)
{
    testing::NiceMock<MockPlugin> plugin;
    auto permissible = std::make_shared<FakePermissible>();
    const std::weak_ptr<Permissible> weak = permissible;
    const NotNull<PermissionAttachment> attachment = permissible->addAttachment(plugin);

    permissible.reset();
    EXPECT_TRUE(weak.expired());
    EXPECT_TRUE(attachment->getPermissible() == nullptr);
    EXPECT_FALSE(attachment->remove());
}

TEST(PermissionAttachmentTest, SurvivesItsPermissible)
{
    testing::NiceMock<MockPlugin> plugin;
    auto permissible = std::make_shared<FakePermissible>();
    const NotNull<PermissionAttachment> attachment = permissible->addAttachment(plugin);
    attachment->setPermission("endstone.test", true);

    permissible.reset();
    attachment->setPermission("endstone.other", true);
    EXPECT_EQ(attachment->getPermissions().size(), 2);
}

TEST(PermissionAttachmentTest, StackAllocatedRemoveReturnsFalse)
{
    testing::NiceMock<MockPlugin> plugin;
    const auto permissible = std::make_shared<FakePermissible>();
    PermissionAttachment attachment{plugin, permissible};
    EXPECT_FALSE(attachment.remove());
}

TEST(PermissionAttachmentInfoTest, DoesNotKeepPermissibleAlive)
{
    auto permissible = std::make_shared<FakePermissible>();
    const std::weak_ptr<Permissible> weak = permissible;
    const PermissionAttachmentInfo info{permissible, "endstone.test", nullptr, true};

    permissible.reset();
    EXPECT_TRUE(weak.expired());
    EXPECT_TRUE(info.getPermissible() == nullptr);
}

TEST(PermissionTest, UnregisteredPermissionHasNoPermissibles)
{
    const auto perm = std::make_shared<Permission>("endstone.test");
    EXPECT_TRUE(perm->getPermissibles().empty());
    EXPECT_THROW(auto parent = perm->addParent("endstone", true), std::runtime_error);
}
