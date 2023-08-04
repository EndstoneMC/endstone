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
    virtual void onEnable(){};
    virtual void onDisable(){};
};

class PythonPlugin : public BasePlugin
{
  public:
    using BasePlugin::BasePlugin; // Inherit the constructors
    void onEnable() override;
    void onDisable() override;
};

#endif // ENDSTONE_PLUGIN_H
