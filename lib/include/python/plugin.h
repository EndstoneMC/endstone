//
// Created by Vincent on 21/07/2023.
//

#ifndef ENDSTONE_PLUGIN_H
#define ENDSTONE_PLUGIN_H

class Plugin
{
  public:
    Plugin() = default;
    virtual ~Plugin() = default;
    virtual void onLoad(){};
    virtual void onEnable(){};
    virtual void onDisable(){};
};

class PyPlugin : public Plugin
{
  public:
    using Plugin::Plugin; // Inherit the constructors
    void onLoad() override;
    void onEnable() override;
    void onDisable() override;
};

#endif // ENDSTONE_PLUGIN_H
