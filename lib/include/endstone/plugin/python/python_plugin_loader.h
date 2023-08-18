//
// Created by Vincent on 18/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_LOADER_H
#define ENDSTONE_PYTHON_PLUGIN_LOADER_H

#include "endstone/plugin/plugin_loader.h"

class PyPluginLoader : public PluginLoader
{
  public:
    Plugin *loadPlugin(const std::string &file) override;
    std::vector<std::string> getPluginFilters() override;
    void enablePlugin(const Plugin &plugin) override;
    void disablePlugin(const Plugin &plugin) override;
};

#endif // ENDSTONE_PYTHON_PLUGIN_LOADER_H
