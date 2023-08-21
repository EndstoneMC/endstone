//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_CPP_PLUGIN_DESCRIPTION_H
#define ENDSTONE_CPP_PLUGIN_DESCRIPTION_H

#include "endstone/plugin/plugin_description.h"

class CppPluginDescription : public PluginDescription
{
  public:
    CppPluginDescription(std::string name, std::string version) : name_(std::move(name)), version_(std::move(version))
    {
    }

    std::string getName() final
    {
        return name_;
    };

    std::string getVersion() final
    {
        return version_;
    };

    std::optional<std::string> getDescription() override
    {
        return {};
    }

    std::optional<std::vector<std::string>> getAuthors() override
    {
        return {};
    }

    std::optional<std::string> getPrefix() override
    {
        return {};
    }

    std::string getFullName() final
    {
        return getName() + " v" + getVersion();
    }

  private:
    std::string name_;
    std::string version_;
};

#endif // ENDSTONE_CPP_PLUGIN_DESCRIPTION_H
