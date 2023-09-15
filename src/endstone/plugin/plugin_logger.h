//
// Created by Vincent on 22/08/2023.
//

#pragma once

#include "endstone/logger.h"
#include "endstone/plugin/plugin.h"

class PluginLogger : public Logger {
public:
    explicit PluginLogger(const Plugin &plugin);
    void setLevel(Level level) override;
    bool isEnabledFor(Level level) const noexcept override;
    std::string_view getName() const override;
    void log(Level level, const std::string &message) const override;

private:
    std::shared_ptr<Logger> logger_;
    std::string pluginName_;
};

