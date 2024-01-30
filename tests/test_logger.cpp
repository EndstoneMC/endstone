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

#include <gtest/gtest.h>

#include "endstone/logger.h"
#include "endstone_core/logger_factory.h"

class LoggerFactoryTest : public ::testing::Test {};

TEST_F(LoggerFactoryTest, CreateLogger)
{
    auto &logger = LoggerFactory::getLogger("TestLogger");
    ASSERT_EQ("TestLogger", logger.getName());
}

TEST_F(LoggerFactoryTest, GetLogger)
{
    auto &logger1 = LoggerFactory::getLogger("TestLogger");
    auto &logger2 = LoggerFactory::getLogger("TestLogger");
    ASSERT_EQ(&logger1, &logger2);
}

TEST_F(LoggerFactoryTest, SetLevel)
{
    auto &logger = LoggerFactory::getLogger("TestLogger");
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Trace));
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Debug));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Info));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Warning));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Error));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Critical));

    logger.setLevel(Logger::Level::Debug);
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Trace));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Debug));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Info));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Warning));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Error));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Critical));

    logger.setLevel(Logger::Level::Error);
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Trace));
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Debug));
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Info));
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Warning));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Error));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Critical));
}
