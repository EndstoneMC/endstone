//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_PERMISSIBLE_H
#define ENDSTONE_PERMISSIBLE_H

#include "endstone/common.h"
#include "server_operator.h"

class Permission;

/**
 * Represents an object that may be assigned permissions
 */
class Permissible : public ServerOperator {
public:
    virtual ~Permissible() = default;

    /**
     * Checks if this object contains an override for the specified
     * permission, by fully qualified name
     *
     * @param name Name of the permission
     * @return true if the permission is set, otherwise false
     */
    virtual bool isPermissionSet(const std::string &name) const = 0;

    /**
     * Checks if this object contains an override for the specified Permission
     *
     * @param perm Permission to check
     * @return true if the permission is set, otherwise false
     */
    virtual bool isPermissionSet(const Permission &perm) const = 0;

    /**
     * Gets the value of the specified permission, if set.
     *
     * If a permission override is not set on this object, the default value
     * of the permission will be returned.
     *
     * @param name Name of the permission
     * @return Value of the permission
     */
    virtual bool hasPermission(const std::string &name) const = 0;

    /**
     * Gets the value of the specified permission, if set.
     *
     * If a permission override is not set on this object, the default value
     * of the permission will be returned
     *
     * @param perm Permission to get
     * @return Value of the permission
     */
    virtual bool hasPermission(const Permission &perm) const = 0;

    /**
     * Recalculates the permissions for this object, if the attachments have
     * changed values.
     *
     * This should very rarely need to be called from a plugin.
     */
    virtual void recalculatePermissions() = 0;
};

#endif // ENDSTONE_PERMISSIBLE_H
