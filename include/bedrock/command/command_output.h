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
    int count;         // +32
};

class CommandOutput {
public:
    void success();
    void forceOutput(const std::string &message_id, const std::vector<CommandOutputParameter> &params);
    void error(const std::string &message_id, const std::vector<CommandOutputParameter> &params);

private:
    BEDROCK_API void addMessage(const std::string &message_id, const std::vector<CommandOutputParameter> &params,
                                enum CommandOutputMessageType);

    CommandOutputType type_;                          // +0
    std::unique_ptr<class CommandPropertyBag> data_;  // +8
    std::vector<CommandOutputMessage> messages_;      // +16
    int success_count_;                               // +40
    bool has_player_text_;                            // +44
};

class CommandOutputSender {};
