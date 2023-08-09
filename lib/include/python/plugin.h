//
// Created by Vincent on 21/07/2023.
//

#ifndef ENDSTONE_PLUGIN_H
#define ENDSTONE_PLUGIN_H

#include "logger.h"
class Plugin
{
  public:
    explicit Plugin() : enabled_(false){};
    virtual ~Plugin() = default;
    virtual void onLoad(){};
    virtual void onEnable(){};
    virtual void onDisable(){};
    bool isEnabled() const;
    void setEnabled(bool enabled);
    virtual Logger &getLogger() = 0;

  private:
    bool enabled_;
};

class PyPlugin : public Plugin
{
  public:
    using Plugin::Plugin; // Inherit the constructors
    void onLoad() override;
    void onEnable() override;
    void onDisable() override;
    Logger &getLogger() override;
};

#endif // ENDSTONE_PLUGIN_H
