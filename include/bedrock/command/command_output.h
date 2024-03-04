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

#include <memory>
#include <string>
#include <vector>

#include "bedrock/bedrock.h"

enum class CommandOutputMessageType {
    Success = 0,
    Error = 1,
};

enum class CommandOutputType {
    None = 0,
    Message = 1,
    Json = 4,
};

class CommandOutputMessage {
public:
    CommandOutputMessageType type;    // +0
    std::string message_id;           // +8
    std::vector<std::string> params;  // +40
};

class CommandOutputParameter {
public:
    std::string text;  // +0
    int count;         // +32 - the number of targets returned by selector (e.g. actor / player)
};

class CommandOutput {
public:
    void success()
    {
        success_count++;
    }
    BEDROCK_API void forceOutput(const std::string &message_id, const std::vector<CommandOutputParameter> &params);
    BEDROCK_API void error(const std::string &message_id, const std::vector<CommandOutputParameter> &params);

    CommandOutputType type;                          // +0
    std::unique_ptr<class CommandPropertyBag> data;  // +8
    std::vector<CommandOutputMessage> messages;      // +16
    int success_count;                               // +40
    bool has_player_text;                            // +44
};

class CommandOutputSender {};
