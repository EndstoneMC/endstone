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
    virtual void onLoad() = 0;
    virtual void onEnable() = 0;
    virtual void onDisable() = 0;
    virtual std::shared_ptr<Logger> getLogger() const = 0;
    virtual bool isEnabled() const = 0;
    virtual std::shared_ptr<const PluginLoader> getPluginLoader() const = 0;
};

#define ENDSTONE_PLUGIN_CLASS(ClassName)              \
    extern "C" ENDSTONE_API CppPlugin *createPlugin() \
    {                                                 \
        return new ClassName();                       \
    }

#endif // ENDSTONE_PLUGIN_H
