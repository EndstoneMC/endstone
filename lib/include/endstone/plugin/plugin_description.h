//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PLUGIN_DESCRIPTION_H
#define ENDSTONE_PLUGIN_DESCRIPTION_H

#include "endstone/common.h"

class PluginDescription
{
  public:
    virtual ~PluginDescription() = default;

    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual std::optional<std::string> getDescription() const = 0;
    virtual std::optional<std::vector<std::string>> getAuthors() const = 0;
    virtual std::optional<std::string> getPrefix() const = 0;
    virtual std::string getFullName() const = 0;
};

#endif // ENDSTONE_PLUGIN_DESCRIPTION_H
