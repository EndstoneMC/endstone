//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_CPP_PLUGIN_DESCRIPTION_H
#define ENDSTONE_CPP_PLUGIN_DESCRIPTION_H

#include <utility>

#include "endstone/plugin/plugin_description.h"

class CppPluginDescription : public PluginDescription
{
  public:
    CppPluginDescription(const std::string &name, const std::string &version)
    {
        if (!std::regex_match(name, VALID_NAME))
        {
            throw std::invalid_argument("Plugin name contains invalid characters.");
        }

        name_ = name;
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = version;
    }

    std::string getName() const final
    {
        return name_;
    };

    std::string getVersion() const final
    {
        return version_;
    };

    std::optional<std::string> getDescription() const override
    {
        return {};
    }

    std::optional<std::vector<std::string>> getAuthors() const override
    {
        return {};
    }

    std::optional<std::string> getPrefix() const override
    {
        return {};
    }

    std::string getFullName() const final
    {
        return getName() + " v" + getVersion();
    }

  private:
    std::string name_;
    std::string version_;

    inline static const std::regex VALID_NAME{"^[A-Za-z0-9 _.-]+$"};
};

#endif // ENDSTONE_CPP_PLUGIN_DESCRIPTION_H
