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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/core/plugin/service_manager.h"
#include "endstone/endstone.hpp"

using namespace endstone;
using namespace endstone::core;

// Mock classes to represent Plugin and Service
class MockPlugin : public endstone::Plugin {
public:
    MOCK_METHOD(const endstone::PluginDescription &, getDescription, (), (const, override));
};

class MockService : public Service {};

// Test fixture for EndstoneServiceManager
class EndstoneServiceManagerTest : public ::testing::Test {
protected:
    EndstoneServiceManager service_manager_;

    std::shared_ptr<Service> high_priority_service_;
    std::shared_ptr<Service> medium_priority_service_;
    std::shared_ptr<Service> low_priority_service_;

    MockPlugin plugin1;
    MockPlugin plugin2;

    void SetUp() override
    {
        high_priority_service_ = std::make_shared<MockService>();
        medium_priority_service_ = std::make_shared<MockService>();
        low_priority_service_ = std::make_shared<MockService>();
    }

    void TearDown() override {}
};

TEST_F(EndstoneServiceManagerTest, RegisterService)
{
    service_manager_.registerService("TestService", high_priority_service_, plugin1, ServicePriority::High);
    auto service = service_manager_.get("TestService");
    ASSERT_NE(service, nullptr);
    ASSERT_EQ(service, high_priority_service_);
}

TEST_F(EndstoneServiceManagerTest, RegisterServicesWithDifferentPriority)
{
    service_manager_.registerService("TestService", low_priority_service_, plugin1, ServicePriority::Low);
    service_manager_.registerService("TestService", high_priority_service_, plugin1, ServicePriority::High);
    auto service = service_manager_.get("TestService");
    ASSERT_EQ(service, high_priority_service_);
}

TEST_F(EndstoneServiceManagerTest, UnregisterService)
{
    service_manager_.registerService("TestService", high_priority_service_, plugin1, ServicePriority::High);
    service_manager_.registerService("TestService", medium_priority_service_, plugin1, ServicePriority::Normal);
    service_manager_.unregister("TestService", *high_priority_service_);
    auto service = service_manager_.get("TestService");
    ASSERT_EQ(service, medium_priority_service_);
}

TEST_F(EndstoneServiceManagerTest, UnregisterAllServicesFromMultiplePlugins)
{
    service_manager_.registerService("TestService", high_priority_service_, plugin1, ServicePriority::High);
    service_manager_.registerService("TestService", medium_priority_service_, plugin2, ServicePriority::Normal);
    service_manager_.unregisterAll(plugin1);
    auto service = service_manager_.get("TestService");
    ASSERT_EQ(service, medium_priority_service_);  // Only plugin2's service remains
    service_manager_.unregisterAll(plugin2);
    service = service_manager_.get("TestService");
    ASSERT_EQ(service, nullptr);  // No service should remain
}

TEST_F(EndstoneServiceManagerTest, UnregisterAllServices)
{
    service_manager_.registerService("TestService", low_priority_service_, plugin1, ServicePriority::Low);
    service_manager_.unregisterAll(plugin1);
    auto service = service_manager_.get("TestService");
    ASSERT_EQ(service, nullptr);
}

TEST_F(EndstoneServiceManagerTest, RetrieveNonExistentService)
{
    auto service = service_manager_.get("NonExistentService");
    ASSERT_EQ(service, nullptr);
}

TEST_F(EndstoneServiceManagerTest, PrioritiesSortedCorrectly)
{
    service_manager_.registerService("TestService", low_priority_service_, plugin1, ServicePriority::Low);
    service_manager_.registerService("TestService", medium_priority_service_, plugin1, ServicePriority::Normal);
    service_manager_.registerService("TestService", high_priority_service_, plugin1, ServicePriority::High);

    auto service = service_manager_.get("TestService");
    ASSERT_EQ(service, high_priority_service_);

    service_manager_.unregister("TestService", *high_priority_service_);
    service = service_manager_.get("TestService");
    ASSERT_EQ(service, medium_priority_service_);

    service_manager_.unregister("TestService", *medium_priority_service_);
    service = service_manager_.get("TestService");
    ASSERT_EQ(service, low_priority_service_);
}

TEST_F(EndstoneServiceManagerTest, UnregisterNonExistentService)
{
    service_manager_.unregister("NonExistentService", *low_priority_service_);
    auto service = service_manager_.get("NonExistentService");
    ASSERT_EQ(service, nullptr);
}
