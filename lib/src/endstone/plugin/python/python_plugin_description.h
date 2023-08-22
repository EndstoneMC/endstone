//
// Created by Vincent on 22/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_DESCRIPTION_H
#define ENDSTONE_PYTHON_PLUGIN_DESCRIPTION_H

#include "endstone/plugin/plugin_description.h"
#include "endstone/pybind.h"

class PythonPluginDescription : public PluginDescription
{
  public:
    explicit PythonPluginDescription(py::object impl);
    ~PythonPluginDescription() override;
    std::string getName() const override;
    std::string getVersion() const override;
    std::optional<std::string> getDescription() const override;
    std::optional<std::vector<std::string>> getAuthors() const override;
    std::optional<std::string> getPrefix() const override;
    std::string getFullName() const override;

  private:
    py::object impl_;
};

#endif // ENDSTONE_PYTHON_PLUGIN_DESCRIPTION_H
