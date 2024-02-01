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

#include "endstone_core/spdlog/spdlog_adapter.h"

SpdLogAdapter::SpdLogAdapter(std::shared_ptr<spdlog::logger> logger) : logger_(std::move(logger)) {}

void SpdLogAdapter::log(Logger::Level level, const std::string &message) const
{
    if (isEnabledFor(level)) {
        logger_->log(static_cast<spdlog::level::level_enum>(level), message);
    }
}

void SpdLogAdapter::setLevel(Logger::Level level)
{
    logger_->set_level(static_cast<spdlog::level::level_enum>(level));
}

bool SpdLogAdapter::isEnabledFor(Logger::Level level) const
{
    return logger_->should_log(static_cast<spdlog::level::level_enum>(level));
}

std::string_view SpdLogAdapter::getName() const
{
    return logger_->name();
}
