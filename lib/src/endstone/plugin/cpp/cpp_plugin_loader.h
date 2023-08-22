//
// Created by Vincent on 21/08/2023.
//

#ifndef ENDSTONE_CPP_PLUGIN_LOADER_H
#define ENDSTONE_CPP_PLUGIN_LOADER_H

#include "endstone/plugin/plugin_loader.h"

class CppPluginLoader : public PluginLoader, public std::enable_shared_from_this<CppPluginLoader>
{
  public:
    ~CppPluginLoader() override = default;
    Plugin *loadPlugin(const std::string &file) const override;
    std::vector<std::string> getPluginFilters() const noexcept override;
    void enablePlugin(Plugin &plugin) const override;
    void disablePlugin(Plugin &plugin) const override;
};

#endif // ENDSTONE_CPP_PLUGIN_LOADER_H
