//
// Created by Vincent on 22/08/2023.
//

#include "plugin_logger.h"

#include "endstone/logger_factory.h"

PluginLogger::PluginLogger(const Plugin &plugin)
{
    auto &description = plugin.getDescription();
    logger_ = LoggerFactory::getLogger(description.getName());

    auto prefix = description.getPrefix();
    pluginName_ = fmt::format("[{}] ", prefix.value_or(description.getName()));
}

void PluginLogger::log(Level level, const std::string &message) const
{
    logger_->log(level, pluginName_ + message);
}

void PluginLogger::setLevel(Level level)
{
    logger_->setLevel(level);
}

bool PluginLogger::isEnabledFor(Level level) const noexcept
{
    return logger_->isEnabledFor(level);
}

std::string_view PluginLogger::getName() const
{
    return logger_->getName();
}
