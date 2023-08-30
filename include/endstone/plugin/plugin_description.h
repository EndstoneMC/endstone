//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PLUGIN_DESCRIPTION_H
#define ENDSTONE_PLUGIN_DESCRIPTION_H

#include "endstone/command/command.h"
#include "endstone/common.h"

class PluginDescription {
public:
    // Constructor for PluginDescription with default behavior from CppPluginDescription
    PluginDescription(const std::string &name, const std::string &version)
    {
        if (!std::regex_match(name, VALID_NAME)) {
            throw std::invalid_argument("Plugin name contains invalid characters.");
        }

        name_ = name;
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = version;
        full_name_ = name_ + " v" + version;
    }

    virtual ~PluginDescription() = default;

    const std::string &getName() const
    {
        return name_;
    }

    const std::string &getVersion() const
    {
        return version_;
    }

    const std::string &getFullName() const
    {
        return full_name_;
    }

    virtual std::optional<std::string> getDescription() const
    {
        return {};
    }

    virtual std::optional<std::vector<std::string>> getAuthors() const
    {
        return {};
    }

    virtual std::optional<std::string> getPrefix() const
    {
        return {};
    }

    virtual std::vector<std::shared_ptr<Command>> getCommands() const
    {
        return {};
    }

private:
    std::string name_;
    std::string version_;
    std::string full_name_;
    inline static const std::regex VALID_NAME{"^[A-Za-z0-9 _.-]+$"};
};

#endif // ENDSTONE_PLUGIN_DESCRIPTION_H
