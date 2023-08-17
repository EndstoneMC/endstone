//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_H
#define ENDSTONE_PYTHON_PLUGIN_H

#include "endstone/plugin/plugin.h"

class PyPlugin : public Plugin
{
  public:
    PluginDescription &getDescription() const override;
    void onLoad() override;
    void onEnable() override;
    void onDisable() override;
    Logger &getLogger() override;
    bool isEnabled() const override;
};

#endif // ENDSTONE_PYTHON_PLUGIN_H
