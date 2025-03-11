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

#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "bedrock/diagnostics/log_area.h"
#include "bedrock/diagnostics/log_level.h"

namespace Bedrock {

struct CallStack {
    struct Frame {
        std::size_t filename_hash;
        std::string_view filename;
        std::uint32_t line;
    };
    BEDROCK_STATIC_ASSERT_SIZE(Frame, 32, 32);

    struct Context {
        Context(std::string value, std::optional<LogLevel> log_level = std::nullopt,
                std::optional<LogAreaID> log_area = std::nullopt)
            : value(std::move(value)), log_level(log_level), log_area(log_area)
        {
        }
        std::string value;
        std::optional<LogLevel> log_level;
        std::optional<LogAreaID> log_area;
    };
    BEDROCK_STATIC_ASSERT_SIZE(Context, 48, 40);

    struct FrameWithContext {
        FrameWithContext(Frame &&frame, std::optional<Context> &&context) : frame(frame), context(std::move(context)) {}
        Frame frame;                     // +0
        std::optional<Context> context;  // +40
    };
    BEDROCK_STATIC_ASSERT_SIZE(FrameWithContext, 88, 80);

    CallStack(FrameWithContext &&frame)
    {
        frames.emplace_back(std::move(frame));
    }

    CallStack(std::vector<FrameWithContext> &&frames)
    {
        frames = std::move(frames);
    }

    std::vector<FrameWithContext> frames;
};

}  // namespace Bedrock
