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
    friend class CppPluginLoader;

    PluginDescription &getDescription() const override = 0;
    void onLoad() override{};
    void onEnable() override{};
    void onDisable() override{};

    Logger &getLogger() const final
    {
        return *logger_;
    }

    bool isEnabled() const final
    {
        return enabled_;
    }

    std::shared_ptr<const PluginLoader> getPluginLoader() const final
    {
        return loader_.lock();
    }

  protected:
    void setEnabled(bool enabled)
    {
        if (enabled_ != enabled)
        {
            enabled_ = enabled;

            if (enabled_)
            {
                onEnable();
            }
            else
            {
                onDisable();
            }
        }
    }

  private:
    bool enabled_{false};
    std::unique_ptr<Logger> logger_{};
    std::weak_ptr<PluginLoader> loader_{};
};

#endif // ENDSTONE_CPP_PLUGIN_H
