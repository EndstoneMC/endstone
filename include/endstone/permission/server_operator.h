//
// Created by Vincent on 12/09/2023.
//

#ifndef SERVER_OPERATOR_H
#define SERVER_OPERATOR_H

/**
 * Represents an object that may become a server operator.
 */
class ServerOperator {
public:
    virtual ~ServerOperator() = default;

    /**
     * Checks if this object is a server operator
     *
     * @return true if this is an operator, otherwise false
     */
    virtual bool isOperator() const = 0;

    /**
     * Sets the operator status of this object
     *
     * @param value New operator value
     */
    virtual void setOperator(bool value) = 0;
};

#endif // SERVER_OPERATOR_H
