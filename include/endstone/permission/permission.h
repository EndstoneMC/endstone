//
// Created by Vincent on 11/09/2023.
//

#ifndef PERMISSION_H
#define PERMISSION_H

#include "endstone/endstone.h"
#include "endstone/plugin/plugin_manager.h"
#include "permissible.h"
#include "permission_default.h"

/**
 * Represents a unique permission that may be attached to a Permissible
 */
class Permission {
private:
    inline static const PermissionDefault &DEFAULT_PERMISSION = PermissionDefault::OP;

    std::string name_;
    std::map<std::string, bool> children_;
    PermissionDefault default_value_ = DEFAULT_PERMISSION;
    std::string description_;

public:
    explicit Permission(const std::string &name) : Permission(name, "", DEFAULT_PERMISSION, {}) {}

    explicit Permission(const std::string &name, const std::string &description)
        : Permission(name, description, DEFAULT_PERMISSION, {})
    {
    }

    explicit Permission(const std::string &name, const PermissionDefault &defaultValue)
        : Permission(name, "", defaultValue, {})
    {
    }

    explicit Permission(const std::string &name, const std::string &description, const PermissionDefault &defaultValue)
        : Permission(name, description, defaultValue, {})
    {
    }

    explicit Permission(const std::string &name, const std::map<std::string, bool> &children)
        : Permission(name, "", DEFAULT_PERMISSION, children)
    {
    }

    explicit Permission(const std::string &name, const std::string &description,
                        const std::map<std::string, bool> &children)
        : Permission(name, description, DEFAULT_PERMISSION, children)
    {
    }

    explicit Permission(const std::string &name, const PermissionDefault &defaultValue,
                        const std::map<std::string, bool> &children)
        : Permission(name, "", defaultValue, children)
    {
    }

    explicit Permission(const std::string &name, const std::string &description, const PermissionDefault &defaultValue,
                        const std::map<std::string, bool> &children)
        : name_(name), description_(description), default_value_(defaultValue), children_(children)
    {
    }

    /**
     * Returns the unique fully qualified name of this Permission
     *
     * @return Fully qualified name
     */
    const std::string &getName() const
    {
        return name_;
    }

    /**
     * Gets the children of this permission.
     *
     * @return Permission children
     */
    const std::map<std::string, bool> &getChildren() const
    {
        return children_;
    }

    /**
     * Gets the default value of this permission.
     *
     * @return Default value of this permission.
     */
    const PermissionDefault &getDefault() const
    {
        return default_value_;
    }

    /**
     * Sets the default value of this permission.
     *
     * This will not be saved to disk, and is a temporary operation until the
     * server reloads permissions. Changing this default will cause all
     * Permissibles that contain this permission to recalculate their
     * permissions
     *
     * @param value The new default to set
     */
    void setDefault(const PermissionDefault &value)
    {
        default_value_ = value;
        recalculatePermissibles();
    }

    /**
     * Gets a brief description of this permission, may be empty
     *
     * @return Brief description of this permission
     */
    const std::string &getDescription() const
    {
        return description_;
    }

    /**
     * Sets the description of this permission.
     *
     * This will not be saved to disk, and is a temporary operation until the
     * server reloads permissions.
     *
     * @param value The new description to set
     */
    void setDescription(const std::string &value)
    {
        description_ = value;
    }

    /**
     * Gets a set containing every Permissible that has this permission.
     *
     * This set cannot be modified.
     *
     * @return Set containing permissibles with this permission
     */
    std::vector<Permissible> getPermissibles() const
    {
        return Endstone::getServer().getPluginManager().getPermissionSubscriptions(name_);
    }

    /**
     * Recalculates all Permissibles that contain this permission.
     *
     * This should be called after modifying the children, and is
     * automatically called after modifying the default value
     */
    void recalculatePermissibles()
    {
        auto perms = getPermissibles();
        Endstone::getServer().getPluginManager().recalculatePermissionDefaults(*this);

        for (auto &p : perms) {
            p.recalculatePermissions();
        }
    }


};

#endif // PERMISSION_H
