//
// Created by Vincent on 14/09/2023.
//

#ifndef ENDSTONE_SRC_ENDSTONE_PERMISSION_PERMISSIBLE_BASE_H_
#define ENDSTONE_SRC_ENDSTONE_PERMISSION_PERMISSIBLE_BASE_H_

#include "endstone/permission/permissible.h"
#include "endstone/permission/permission_attachment.h"

class PermissibleBase : public Permissible {

public:
    explicit PermissibleBase(ServerOperator *opable) : opable_(opable), parent(this)
    {
        recalculatePermissions();
    }

    explicit PermissibleBase(Permissible *opable) : opable_(opable), parent(opable)
    {
        recalculatePermissions();
    }

    bool isOperator() const override
    {
        if (!opable_) {
            return false;
        }
        else {
            return opable_->isOperator();
        }
    }

    void setOperator(bool value) override
    {
        if (!opable_) {
            throw std::runtime_error("Cannot change op value as no ServerOperator is set");
        }
        else {
            opable_->setOperator(value);
        }
    }

private:
    ServerOperator *opable_;
    Permissible *parent;
};

#endif // ENDSTONE_SRC_ENDSTONE_PERMISSION_PERMISSIBLE_BASE_H_
