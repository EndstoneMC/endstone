//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_CPP_PLUGIN_H
#define ENDSTONE_CPP_PLUGIN_H

#include "cpp_plugin_description.h"
#include "endstone/plugin/plugin.h"

class CppPlugin : public Plugin
{
  public:
    CppPluginDescription &getDescription() const override = 0;
    void onLoad() override;
    void onEnable() override;
    void onDisable() override;
    Logger &getLogger() override;
    bool isEnabled() const override;

  protected:
    void setEnabled(bool enabled);

  private:
    bool enabled_;
    std::unique_ptr<Logger> logger_;
};

#endif // ENDSTONE_CPP_PLUGIN_H
