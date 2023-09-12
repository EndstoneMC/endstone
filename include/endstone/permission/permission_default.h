//
// Created by Vincent on 11/09/2023.
//

#ifndef PERMISSION_DEFAULT_H
#define PERMISSION_DEFAULT_H

#include "endstone/common.h"

/**
 * Represents the possible default values for permissions
 */
class PermissionDefault {
private:
    enum class Value {
        True,
        False,
        Operator,
        NotOperator
    };

    explicit PermissionDefault(Value value, const std::vector<std::string> &names) : value_(value), names_(names) {
        for (const auto &name: names) {
            lookup_.insert({name, this});
        }
    }

    std::vector<std::string> names_;
    Value value_;
    static std::unordered_map<std::string, PermissionDefault *> lookup_;

public:
    /**
     * Calculates the value of this PermissionDefault for the given operator value
     *
     * @param op If the target is op
     * @return True if the default should be true, or false
     */
    bool getValue(bool op) {
        switch (value_) {
            case Value::True:
                return true;
            case Value::False:
                return false;
            case Value::Operator:
                return op;
            case Value::NotOperator:
                return !op;
            default:
                return false;
        }
    }

    /**
     * Looks up a PermissionDefault by name
     *
     * @param name Name of the default
     * @return Specified value, or nullptr if not found
     */
    static PermissionDefault *getByName(std::string name) {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        name = std::regex_replace(name, std::regex("[^a-z!]"), "");
        if (lookup_.find(name) != lookup_.end()) {
            return lookup_[name];
        }
        return nullptr;
    }

    static const PermissionDefault TRUE;
    static const PermissionDefault FALSE;
    static const PermissionDefault OP;
    static const PermissionDefault NOT_OP;
};

std::unordered_map<std::string, PermissionDefault *> PermissionDefault::lookup_;

const PermissionDefault PermissionDefault::TRUE(Value::True, {"true"});
const PermissionDefault PermissionDefault::FALSE(Value::False, {"false"});
const PermissionDefault PermissionDefault::OP(Value::Operator,
                                              {"op", "isop", "operator", "isoperator", "admin", "isadmin"});
const PermissionDefault PermissionDefault::NOT_OP(Value::NotOperator,
                                                  {"!op", "notop", "!operator", "notoperator", "!admin", "notadmin"});


#endif //PERMISSION_DEFAULT_H
