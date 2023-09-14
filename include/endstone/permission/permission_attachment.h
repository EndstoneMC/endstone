//
// Created by Vincent on 14/09/2023.
//

#ifndef ENDSTONE_INCLUDE_ENDSTONE_PERMISSION_PERMISSION_ATTACHMENT_H_
#define ENDSTONE_INCLUDE_ENDSTONE_PERMISSION_PERMISSION_ATTACHMENT_H_

#include "endstone/plugin/plugin.h"
class PermissionAttachment {
private:
    PermissionRemovedExecutor *removed = nullptr;
    std::map<std::string, bool> permissions;
    Permissible &permissible;
    Plugin &plugin;

public:
    PermissionAttachment(Plugin &plugin, Permissible &permissible) : plugin(plugin), permissible(permissible)
    {
        if (!plugin.isEnabled()) {
            throw std::invalid_argument("Plugin is disabled");
        }
    }

    Plugin &getPlugin() const
    {
        return plugin;
    }

    void setRemovalCallback(PermissionRemovedExecutor *ex)
    {
        removed = ex;
    }

    PermissionRemovedExecutor *getRemovalCallback() const
    {
        return removed;
    }

    Permissible &getPermissible() const
    {
        return permissible;
    }

    std::map<std::string, bool> getPermissions() const
    {
        return permissions;
    }

    void setPermission(const std::string &name, bool value)
    {
        std::string lowercaseName = name;
        std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), ::tolower);
        permissions[lowercaseName] = value;
        permissible.recalculatePermissions();
    }

    void setPermission(const Permission &perm, bool value)
    {
        setPermission(perm.getName(), value);
    }

    void unsetPermission(const std::string &name)
    {
        std::string lowercaseName = name;
        std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), ::tolower);
        permissions.erase(lowercaseName);
        permissible.recalculatePermissions();
    }

    void unsetPermission(const Permission &perm)
    {
        unsetPermission(perm.getName());
    }

    bool remove()
    {
        try {
            permissible.removeAttachment(*this);
            return true;
        }
        catch (const std::invalid_argument &) {
            return false;
        }
    }
};

#endif // ENDSTONE_INCLUDE_ENDSTONE_PERMISSION_PERMISSION_ATTACHMENT_H_
