//
// Created by Vincent on 17/08/2023.
//

#pragma once

#include "plugin.h"

class PluginLoader {
public:
    explicit PluginLoader(Server &server) : server_(server){};

    virtual ~PluginLoader() = default;
    virtual std::unique_ptr<Plugin> loadPlugin(const std::string &file) = 0;
    virtual std::vector<std::string> getPluginFileFilters() const noexcept = 0;
    virtual void enablePlugin(Plugin &plugin) const noexcept
    {
        if (!plugin.isEnabled()) {
            plugin.getLogger()->info("Enabling {}", plugin.getDescription().getFullName());
            plugin.setEnabled(true);
        }
    }
    virtual void disablePlugin(Plugin &plugin) const noexcept
    {
        if (plugin.isEnabled()) {
            plugin.getLogger()->info("Disabling {}", plugin.getDescription().getFullName());
            plugin.setEnabled(false);
        }
    }

protected:
    virtual void initPlugin(Plugin &plugin, const std::shared_ptr<Logger> &logger) noexcept
    {
        plugin.loader_ = this;
        plugin.server_ = &server_;
        plugin.logger_ = logger;
    }

private:
    Server &server_;
};

