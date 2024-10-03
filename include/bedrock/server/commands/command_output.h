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
#include <utility>
#include <vector>

#include "bedrock/server/commands/command_property_bag.h"

enum class CommandOutputMessageType {
    Success = 0,
    Error = 1,
};

enum class CommandOutputType {
    None = 0,
    LastOutput = 1,
    Silent = 2,
    AllOutput = 3,
    DataSet = 4,
};

class CommandOutputMessage {
public:
    CommandOutputMessage(CommandOutputMessageType type, std::string message_id, std::vector<std::string> &&params);

    [[nodiscard]] CommandOutputMessageType getType() const;
    [[nodiscard]] std::string const &getMessageId() const;
    [[nodiscard]] std::vector<std::string> const &getParams() const;

private:
    CommandOutputMessageType type_;    // +0
    std::string message_id_;           // +8
    std::vector<std::string> params_;  // +40
};

class CommandOutputParameter {
public:
    explicit CommandOutputParameter(std::string value) : string_(std::move(value)) {}
    [[nodiscard]] std::string const &getText() const;
    [[nodiscard]] int count() const;

private:
    std::string string_;  // +0
    int count_{0};        // +32
};

class CommandOutput {
public:
    explicit CommandOutput(CommandOutputType type);
    void success();
    void forceOutput(const std::string &message_id, const std::vector<CommandOutputParameter> &params);
    void error(const std::string &message_id, const std::vector<CommandOutputParameter> &params);
    [[nodiscard]] int getSuccessCount() const;
    [[nodiscard]] const std::vector<CommandOutputMessage> &getMessages() const;

private:
    void addMessage(const std::string &message_id, const std::vector<CommandOutputParameter> &params,
                    CommandOutputMessageType);

    CommandOutputType type_;                      // +0
    std::unique_ptr<CommandPropertyBag> data_;    // +8
    std::vector<CommandOutputMessage> messages_;  // +16
    int success_count_{0};                        // +40
    bool has_player_text_{false};                 // +44
};

class CommandOutputSender {};
