//
// Created by Vincent on 21/07/2023.
//

#ifndef ENDSTONE_PLUGIN_H
#define ENDSTONE_PLUGIN_H

#include "endstone/command/command_executor.h"
#include "endstone/logger.h"
#include "endstone/server.h"
#include "plugin_description.h"

class PluginLoader;
class PluginCommand;

class Plugin : public CommandExecutor {
public:
    explicit Plugin() = default;
    ~Plugin() override = default;

    virtual const PluginDescription &getDescription() const = 0;

    /**
     * Called after a plugin is loaded but before it has been enabled.
     *
     * When multiple plugins are loaded, the onLoad() for all plugins is
     * called before any onEnable() is called.
     */
    virtual void onLoad(){};

    /**
     * Called when this plugin is enabled
     */
    virtual void onEnable(){};

    /**
     * Called when this plugin is disabled
     */
    virtual void onDisable(){};

    bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) noexcept override
    {
        return false;
    }

    /**
     * Returns the plugin logger associated with this server's logger. The
     * returned logger automatically tags all log messages with the plugin's
     * name.
     *
     * @return Logger associated with this plugin
     */
    std::shared_ptr<Logger> getLogger()
    {
        return logger_;
    }

    /**
     * Returns a value indicating whether or not this plugin is currently
     * enabled
     *
     * @return true if this plugin is enabled, otherwise false
     */
    bool isEnabled()
    {
        return enabled_;
    }

    /**
     * Gets the associated PluginLoader responsible for this plugin
     *
     * @return PluginLoader that controls this plugin
     */
    PluginLoader &getPluginLoader()
    {
        return *loader_;
    }

    /**
     * Returns the Server instance currently running this plugin
     *
     * @return Server running this plugin
     */
    Server &getServer()
    {
        return *server_;
    }

    /**
     * Gets the command with the given name, specific to this plugin.
     *
     * @param name name or alias of the command
     * @return the plugin command if found, otherwise null
     */
    std::shared_ptr<PluginCommand> getCommand(const std::string &name)
    {
        auto alias = name;
        std::transform(alias.begin(), alias.end(), alias.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        auto command = getServer().getPluginCommand(alias);
        if (!command) {
            auto prefix = getDescription().getName();
            std::transform(prefix.begin(), prefix.end(), prefix.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            command = getServer().getPluginCommand(prefix + ":" + alias);
        }

        return command;
    }

private:
    friend class PluginLoader;

    /**
     * Sets the enabled state of this plugin
     *
     * @param enabled true if enabled, otherwise false
     */
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
    Server *server_{nullptr};
    std::shared_ptr<Logger> logger_;
};

#define ENDSTONE_PLUGIN_CLASS(ClassName)           \
    extern "C" ENDSTONE_API Plugin *createPlugin() \
    {                                              \
        return new ClassName();                    \
    }

#endif // ENDSTONE_PLUGIN_H
