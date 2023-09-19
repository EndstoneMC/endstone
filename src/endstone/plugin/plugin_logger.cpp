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

#include "endstone/plugin/plugin_logger.h"

#include "endstone/logger_factory.h"

PluginLogger::PluginLogger(const Plugin &plugin) : logger_(LoggerFactory::getLogger(plugin.getDescription().getName()))
{
    const auto &description = plugin.getDescription();
    auto prefix = description.getPrefix();
    plugin_name_ = fmt::format("[{}] ", prefix.value_or(description.getName()));
}

void PluginLogger::log(Level level, const std::string &message) const
{
    logger_.log(level, plugin_name_ + message);
}

void PluginLogger::setLevel(Level level)
{
    logger_.setLevel(level);
}

bool PluginLogger::isEnabledFor(Level level) const noexcept
{
    return logger_.isEnabledFor(level);
}

std::string_view PluginLogger::getName() const
{
    return logger_.getName();
}
