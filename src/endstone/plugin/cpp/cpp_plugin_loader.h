//
// Created by Vincent on 21/08/2023.
//

#pragma once

#include "endstone/plugin/plugin_loader.h"

class CppPluginLoader : public PluginLoader {
public:
    using PluginLoader::PluginLoader;

    std::unique_ptr<Plugin> loadPlugin(const std::string &file) override;
    std::vector<std::string> getPluginFileFilters() const noexcept override;
};

