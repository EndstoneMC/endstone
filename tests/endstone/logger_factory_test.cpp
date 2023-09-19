//
// Created by Vincent on 24/08/2023.
//

//
// Created by Vincent on 23/08/2023.
//

#include <gtest/gtest.h>

#include "bedrock/bedrock_log.h"
#include "endstone/logger_factory.h"

std::string gLastMessage;

BEDROCK_API void BedrockLog::log_va(BedrockLog::LogCategory /*category*/, std::bitset<3> /*flags*/,
                                    BedrockLog::LogRule /*rule*/, LogAreaID /*area*/, LogLevel /*level*/,
                                    char const * /*function*/, int /*line*/, char const *format, va_list args)
{
    va_list tmp_args;  // Required to calculate the needed length without modifying the original va_list.
    va_copy(tmp_args, args);
    auto len = vsnprintf(nullptr, 0, format, tmp_args) + 1;
    va_end(tmp_args);

    gLastMessage.resize(len - 1);
    vsnprintf(gLastMessage.data(), len, format, args);
    va_end(args);
}

TEST(BedrockLoggerAdapterTest, InitializeWithName)
{
    BedrockLoggerAdapter logger("TestLogger");
    EXPECT_EQ(logger.getName(), "TestLogger");
}

TEST(BedrockLoggerAdapterTest, SetAndGetLevel)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.setLevel(Logger::Level::Error);
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Error));
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Info));
}

TEST(BedrockLoggerAdapterTest, LogEnabledForLevel)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.setLevel(Logger::Level::Warning);
    ASSERT_FALSE(logger.isEnabledFor(Logger::Level::Info));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Warning));
    ASSERT_TRUE(logger.isEnabledFor(Logger::Level::Error));
}

TEST(BedrockLoggerAdapterTest, LogMessage)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.log(Logger::Level::Info, "This is a test message.");
    ASSERT_EQ(gLastMessage, "This is a test message.");
}

TEST(BedrockLoggerAdapterTest, LogOnlyForEnabledLevels)
{
    BedrockLoggerAdapter logger("TestLogger");
    logger.setLevel(Logger::Level::Error);  // Setting the log level to Error

    // Try to log a message with Info level (shouldn't be logged due to the level check)
    logger.log(Logger::Level::Info, "This is an info message.");
    ASSERT_NE(gLastMessage, "This is an info message.");

    // Now, log a message with Error level (should be logged)
    logger.log(Logger::Level::Error, "This is an error message.");
    ASSERT_EQ(gLastMessage, "This is an error message.");
}

TEST(LoggerFactoryTest, ReturnLoggerForNewName)
{
    auto &logger = LoggerFactory::getLogger("UniqueLogger");
    ASSERT_EQ(logger.getName(), "UniqueLogger");
}

TEST(LoggerFactoryTest, ReturnSameLoggerForExistingName)
{
    auto &first_logger = LoggerFactory::getLogger("SameLogger");
    auto &second_logger = LoggerFactory::getLogger("SameLogger");
    ASSERT_EQ(&first_logger, &second_logger);
}
