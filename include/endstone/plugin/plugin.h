//
// Created by Vincent on 21/07/2023.
//

#ifndef ENDSTONE_PLUGIN_H
#define ENDSTONE_PLUGIN_H

#include "endstone/command/command_executor.h"
#include "endstone/logger.h"
#include "plugin_description.h"

class PluginLoader;

class Plugin : public CommandExecutor {
public:
    explicit Plugin() = default;
    ~Plugin() override = default;

    virtual const PluginDescription &getDescription() const = 0;
    virtual void onLoad(){};
    virtual void onEnable(){};
    virtual void onDisable(){};

    bool onCommand(CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) const noexcept override
    {
        return false;
    }

    std::shared_ptr<Logger> getLogger()
    {
        return logger_;
    }

    bool isEnabled()
    {
        return enabled_;
    }

    PluginLoader &getPluginLoader()
    {
        return *loader_;
    }

private:
    friend class PluginLoader;

    void setEnabled(bool enabled)
    {
        if (enabled_ != enabled) {
            enabled_ = enabled;

            if (enabled_) {
                onEnable();
            }
            else {
                onDisable();
            }
        }
    }

private:
    bool enabled_{false};
    PluginLoader *loader_{nullptr};
    std::shared_ptr<Logger> logger_;
};

#define ENDSTONE_PLUGIN_CLASS(ClassName)           \
    extern "C" ENDSTONE_API Plugin *createPlugin() \
    {                                              \
        return new ClassName();                    \
    }

#endif // ENDSTONE_PLUGIN_H
