//
// Created by Vincent on 24/08/2023.
//

//
// Created by Vincent on 23/08/2023.
//

#include "bedrock/bedrock_log.h"
#include "endstone/logger_factory.h"

#include <gtest/gtest.h>

static std::string last_message;

void BedrockLog::log_va(BedrockLog::LogCategory category, std::bitset<3> flags, BedrockLog::LogRule rule,
                        BedrockLog::LogAreaID area, LogLevel level, char const *function, int line, char const *message,
                        va_list args)
{
    last_message = message;
}

TEST(BedrockLoggerAdapterTest, InitializeWithName)
{
    BedrockLoggerAdapter logger("TestLogger");
    EXPECT_EQ(logger.getName(), "TestLogger");
}

TEST(BedrockLoggerAdapterTest, SetAndGetLevel)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.setLevel(LogLevel::Error);
    ASSERT_TRUE(logger.isEnabledFor(LogLevel::Error));
    ASSERT_FALSE(logger.isEnabledFor(LogLevel::Info));
}

TEST(BedrockLoggerAdapterTest, LogEnabledForLevel)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.setLevel(LogLevel::Warning);
    ASSERT_FALSE(logger.isEnabledFor(LogLevel::Info));
    ASSERT_TRUE(logger.isEnabledFor(LogLevel::Warning));
    ASSERT_TRUE(logger.isEnabledFor(LogLevel::Error));
}

TEST(BedrockLoggerAdapterTest, LogMessage)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.log(LogLevel::Info, "This is a test message.");
    ASSERT_EQ(last_message, "This is a test message.");
}

TEST(BedrockLoggerAdapterTest, LogOnlyForEnabledLevels)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.setLevel(LogLevel::Error); // Setting the log level to Error

    // Try to log a message with Info level (shouldn't be logged due to the level check)
    logger.log(LogLevel::Info, "This is an info message.");
    ASSERT_NE(last_message, "This is an info message.");

    // Now, log a message with Error level (should be logged)
    logger.log(LogLevel::Error, "This is an error message.");
    ASSERT_EQ(last_message, "This is an error message.");
}

TEST(LoggerFactoryTest, ReturnLoggerForNewName)
{
    auto logger = LoggerFactory::getLogger("UniqueLogger");
    ASSERT_EQ(logger->getName(), "UniqueLogger");
}

TEST(LoggerFactoryTest, ReturnSameLoggerForExistingName)
{
    auto first_logger = LoggerFactory::getLogger("SameLogger");
    auto second_logger = LoggerFactory::getLogger("SameLogger");
    ASSERT_EQ(first_logger, second_logger);
}
