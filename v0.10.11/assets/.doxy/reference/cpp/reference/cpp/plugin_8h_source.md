

# File plugin.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**plugin**](dir_53ee43673b2467e53c4cb8c30a2e7d89.md) **>** [**plugin.h**](plugin_8h.md)

[Go to the documentation of this file](plugin_8h.md)


```C++
// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <algorithm>
#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "endstone/command/command_executor.h"
#include "endstone/logger.h"
#include "endstone/permissions/permission.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/server.h"

namespace endstone {
class PluginCommand;
class PluginLoader;
namespace core {
class EndstonePluginManager;
}

class Plugin : public CommandExecutor {
public:
    Plugin() = default;
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;

    ~Plugin() override = default;

    [[nodiscard]] virtual const PluginDescription &getDescription() const = 0;

    virtual void onLoad() {}

    virtual void onEnable() {}

    virtual void onDisable() {}

    [[nodiscard]] Logger &getLogger() const
    {
        return *logger_;
    }

    [[nodiscard]] bool isEnabled() const
    {
        return enabled_;
    }

    [[nodiscard]] PluginLoader &getPluginLoader() const
    {
        return *loader_;
    }

    [[nodiscard]] Server &getServer() const
    {
        return *server_;
    }

    [[nodiscard]] std::string getName() const
    {
        return getDescription().getName();
    };

    [[nodiscard]] PluginCommand *getCommand(std::string name) const
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        return getServer().getPluginCommand(name);
    }

    [[nodiscard]] const std::filesystem::path &getDataFolder() const
    {
        return data_folder_;
    }

    template <typename EventType, typename T>
    void registerEvent(void (T::*func)(EventType &), T &instance, EventPriority priority = EventPriority::Normal,
                       bool ignore_cancelled = false)
    {
        getServer().getPluginManager().registerEvent(
            EventType::NAME, [func, &instance](Event &e) { (instance.*func)(reinterpret_cast<EventType &>(e)); },
            priority, *this, ignore_cancelled);
    }

    template <typename EventType>
    void registerEvent(std::function<void(EventType &)> func, EventPriority priority = EventPriority::Normal,
                       bool ignore_cancelled = false)
    {
        getServer().getPluginManager().registerEvent(
            EventType::NAME, [func](Event &e) { func(reinterpret_cast<EventType &>(e)); }, priority, *this,
            ignore_cancelled);
    }

protected:
    friend class PluginLoader;
    friend class core::EndstonePluginManager;

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
    Logger *logger_{nullptr};
    std::filesystem::path data_folder_;
};

namespace detail {
class CommandBuilder {
public:
    explicit CommandBuilder(std::string name) : name_(std::move(name)) {}

    CommandBuilder &description(std::string description)
    {
        description_ = std::move(description);
        return *this;
    }

    template <typename... Usage>
    CommandBuilder &usages(Usage... usages)
    {
        (usages_.insert(usages), ...);
        return *this;
    }

    template <typename... Alias>
    CommandBuilder &aliases(Alias... aliases)
    {
        (aliases_.insert(aliases), ...);
        return *this;
    }

    template <typename... Permissions>
    CommandBuilder &permissions(Permissions... permissions)
    {
        (permissions_.insert(permissions), ...);
        return *this;
    }

    [[nodiscard]] Command build() const
    {
        return Command(name_, description_, std::vector<std::string>(usages_.begin(), usages_.end()),
                       std::vector<std::string>(aliases_.begin(), aliases_.end()),
                       std::vector<std::string>(permissions_.begin(), permissions_.end()));
    }

private:
    std::string name_;
    std::string description_;
    std::set<std::string> usages_;
    std::set<std::string> aliases_;
    std::set<std::string> permissions_;
};

class PermissionBuilder {
public:
    explicit PermissionBuilder(std::string name) : name_(std::move(name)) {}

    PermissionBuilder &description(std::string description)
    {
        description_ = std::move(description);
        return *this;
    }

    PermissionBuilder &default_(PermissionDefault default_value)  // NOLINT(*-identifier-naming)
    {
        default_value_ = default_value;
        return *this;
    }

    PermissionBuilder &children(const std::string &name, bool value)
    {
        children_[name] = value;
        return *this;
    }

    [[nodiscard]] Permission build() const
    {
        return Permission(name_, description_, default_value_, children_);
    }

private:
    std::string name_;
    std::string description_;
    PermissionDefault default_value_ = Permission::DefaultPermission;
    std::unordered_map<std::string, bool> children_ = {};
};

struct PluginDescriptionBuilder {
    std::string description;
    PluginLoadOrder load = PluginLoadOrder::PostWorld;
    std::vector<std::string> authors;
    std::vector<std::string> contributors;
    std::string website;
    std::string prefix;
    std::vector<std::string> provides;
    std::vector<std::string> depend;
    std::vector<std::string> soft_depend;
    std::vector<std::string> load_before;
    PermissionDefault default_permission = PermissionDefault::Operator;
    std::unordered_map<std::string, CommandBuilder> commands;
    std::unordered_map<std::string, PermissionBuilder> permissions;

    CommandBuilder &command(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        return commands.emplace(name, name).first->second;
    }

    PermissionBuilder &permission(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        return permissions.emplace(name, name).first->second;
    }

    [[nodiscard]] PluginDescription build(std::string name, std::string version) const
    {
        return {std::move(name),
                std::move(version),
                description,
                load,
                authors,
                contributors,
                website,
                prefix,
                provides,
                depend,
                soft_depend,
                load_before,
                default_permission,
                buildCommands(),
                buildPermissions()};
    }

private:
    [[nodiscard]] std::vector<Command> buildCommands() const
    {
        std::vector<Command> result;
        result.reserve(commands.size());
        for (const auto &pair : commands) {
            result.push_back(pair.second.build());
        }
        return result;
    }

    [[nodiscard]] std::vector<Permission> buildPermissions() const
    {
        std::vector<Permission> result;
        result.reserve(permissions.size());
        for (const auto &pair : permissions) {
            result.push_back(pair.second.build());
        }
        return result;
    }
};
}  // namespace detail

}  // namespace endstone

#ifndef ENDSTONE_EXPORT
#if defined(WIN32) || defined(_WIN32)
#define ENDSTONE_EXPORT __declspec(dllexport)
#else
#define ENDSTONE_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifndef ENDSTONE_PLUGIN
#define ENDSTONE_PLUGIN(Name, Version, MainClass)                                            \
    class PluginDescriptionBuilderImpl : public endstone::detail::PluginDescriptionBuilder { \
    public:                                                                                  \
        PluginDescriptionBuilderImpl();                                                      \
    };                                                                                       \
    static PluginDescriptionBuilderImpl builder;                                             \
    class EndstonePluginImpl : public MainClass {                                            \
    public:                                                                                  \
        EndstonePluginImpl() = default;                                                      \
        const endstone::PluginDescription &getDescription() const override                   \
        {                                                                                    \
            return description_;                                                             \
        }                                                                                    \
                                                                                             \
    private:                                                                                 \
        endstone::PluginDescription description_ = builder.build(Name, Version);             \
    };                                                                                       \
    extern "C" [[maybe_unused]] ENDSTONE_EXPORT endstone::Plugin *init_endstone_plugin()     \
    {                                                                                        \
        auto *p = new EndstonePluginImpl();                                                  \
        return p;                                                                            \
    }                                                                                        \
    PluginDescriptionBuilderImpl::PluginDescriptionBuilderImpl()
#endif
```


