//
// Created by Vincent on 21/07/2023.
//

#ifndef ENDSTONE_PLUGIN_H
#define ENDSTONE_PLUGIN_H

class BasePlugin
{
  public:
    BasePlugin() = default;
    virtual ~BasePlugin() = default;
    virtual void on_enable() = 0;
    virtual void on_disable() = 0;
};

class PythonPlugin : public BasePlugin
{
  public:
    using BasePlugin::BasePlugin; // Inherit the constructors
    void on_enable() override;
    void on_disable() override;
};

#endif // ENDSTONE_PLUGIN_H
