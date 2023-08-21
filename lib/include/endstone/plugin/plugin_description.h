//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PLUGIN_DESCRIPTION_H
#define ENDSTONE_PLUGIN_DESCRIPTION_H

#include "common.h"

class PluginDescription
{
  public:
    virtual ~PluginDescription() = default;

    virtual std::string getName() = 0;
    virtual std::string getVersion() = 0;
    virtual std::optional<std::string> getDescription() = 0;
    virtual std::optional<std::vector<std::string>> getAuthors() = 0;
    virtual std::optional<std::string> getPrefix() = 0;
    virtual std::string getFullName() = 0;
};

#endif // ENDSTONE_PLUGIN_DESCRIPTION_H
