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

#include <string>
#include <system_error>

#include <gtest/gtest.h>

#include "bedrock/platform/check.h"

TEST(ErrorInfoTest, MessageContainsErrorText)
{
    auto error_info = BEDROCK_NEW_ERROR(std::errc::invalid_argument);
    std::string message = error_info.message();
    EXPECT_NE(message.find("Error:"), std::string::npos);
}

TEST(ErrorInfoTest, MessageContainsCallStackContext)
{
    const std::string error_context = "Custom error context";
    auto error_info = BEDROCK_NEW_ERROR_MESSAGE(std::errc::invalid_argument, error_context);
    std::string message = error_info.message();
    EXPECT_NE(message.find(error_context), std::string::npos);
}

TEST(ErrorInfoTest, BranchesAreFormattedCorrectly)
{
    auto parent_error = BEDROCK_NEW_ERROR(std::errc::invalid_argument);
    auto child_error = BEDROCK_NEW_ERROR(std::errc::result_out_of_range);

    // Manually attach the child as a branch.
    parent_error.branches.push_back(child_error);
    std::string message = parent_error.message();

    // Check that the child error message appears in the parent's message.
    auto pos = message.find("Error:");
    EXPECT_NE(pos, std::string::npos);
    pos = message.find("Error:", pos + 1);
    EXPECT_NE(pos, std::string::npos);
}
