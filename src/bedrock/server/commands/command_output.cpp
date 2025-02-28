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

#include "bedrock/server/commands/command_output.h"

#include <utility>

#include "bedrock/locale/i18n.h"

CommandOutputMessage::CommandOutputMessage(CommandOutputMessageType type, std::string message_id,
                                           std::vector<std::string> &&params)
    : type_(type), message_id_(std::move(message_id)), params_(std::move(params))
{
}

CommandOutputMessageType CommandOutputMessage::getType() const
{
    return type_;
}

std::string const &CommandOutputMessage::getMessageId() const
{
    return message_id_;
}

std::vector<std::string> const &CommandOutputMessage::getParams() const
{
    return params_;
}

std::string const &CommandOutputParameter::getText() const
{
    return string_;
}

int CommandOutputParameter::count() const
{
    return count_;
}

CommandOutput::CommandOutput(CommandOutputType type) : type_(type) {}

void CommandOutput::success()
{
    success_count_++;
}

void CommandOutput::forceOutput(const std::string &message_id, const std::vector<CommandOutputParameter> &params)
{
    if (type_ != CommandOutputType::None) {
        addMessage(message_id, params, CommandOutputMessageType::Success);
    }
}

void CommandOutput::error(const std::string &message_id, const std::vector<CommandOutputParameter> &params)
{
    if (type_ != CommandOutputType::None) {
        addMessage(message_id, params, CommandOutputMessageType::Error);
    }
}

int CommandOutput::getSuccessCount() const
{
    return success_count_;
}

CommandOutputType CommandOutput::getType() const
{
    return type_;
}

const std::vector<CommandOutputMessage> &CommandOutput::getMessages() const
{
    return messages_;
}

void CommandOutput::addMessage(const std::string &message_id, const std::vector<CommandOutputParameter> &params,
                               CommandOutputMessageType type)
{
    if (type_ == CommandOutputType::LastOutput) {
        messages_.clear();
    }

    std::vector<std::string> param_list;
    param_list.reserve(params.size());
    for (const auto &param : params) {
        param_list.push_back(param.getText());
    }
    messages_.emplace_back(type, message_id, std::move(param_list));
}
