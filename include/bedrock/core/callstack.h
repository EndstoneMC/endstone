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

#include <optional>

#include "bedrock/core/bedrock_log.h"

namespace Bedrock {

class CallStack {
public:
    class Frame {
    public:
        std::size_t unknown[4];
    };
    BEDROCK_STATIC_ASSERT_SIZE(Frame, 32, 32);

    class Context {
    public:
        std::string message;
        std::optional<Bedrock::LogLevel> log_level;
        std::optional<LogAreaID> log_area;
    };
    BEDROCK_STATIC_ASSERT_SIZE(Context, 48, 40);

    class FrameWithContext {
    public:
        Frame frame;                     // +0
        std::optional<Context> context;  // +40
    };
    BEDROCK_STATIC_ASSERT_SIZE(FrameWithContext, 88, 80);
};

}  // namespace Bedrock
