//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_CPP_PLUGIN_DESCRIPTION_H
#define ENDSTONE_CPP_PLUGIN_DESCRIPTION_H

#include "endstone/plugin/plugin_description.h"

class CppPluginDescription : public PluginDescription
{
  public:
    CppPluginDescription(std::string name, std::string version);
    std::string getName() override;
    std::string getVersion() override;
    std::optional<std::string> getDescription() override;
    std::optional<std::vector<std::string>> getAuthors() override;
    std::optional<std::string> getPrefix() override;
    std::string getFullName() override;

  private:
    std::string name_;
    std::string version_;
};

#endif // ENDSTONE_CPP_PLUGIN_DESCRIPTION_H
