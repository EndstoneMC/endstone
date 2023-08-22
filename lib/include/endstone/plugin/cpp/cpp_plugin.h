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

    const PluginDescription &getDescription() const override = 0;
    void onLoad() override{};
    void onEnable() override{};
    void onDisable() override{};

    std::shared_ptr<Logger> getLogger() const final
    {
        return logger_;
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
    std::weak_ptr<const PluginLoader> loader_;
    std::shared_ptr<Logger> logger_;
};

#endif // ENDSTONE_CPP_PLUGIN_H
