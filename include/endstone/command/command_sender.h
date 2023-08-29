//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_COMMAND_SENDER_H
#define ENDSTONE_COMMAND_SENDER_H

#include "endstone/common.h"
#include "endstone/permission/permissible.h"

class Server;

class CommandSender : public Permissible {
public:
    virtual ~CommandSender() = default;

    /**
     * Sends this sender a message
     *
     * @param message Message to be displayed
     */
    virtual void sendMessage(const std::string &message) const = 0;

    template <typename... Args>
    void sendMessage(const fmt::format_string<Args...> format, Args &&...args) const
    {
        sendMessage(fmt::format(format, std::forward<Args>(args)...));
    }

    /**
     * Returns the server instance that this command is running on
     *
     * @return Server instance
     */
    virtual Server &getServer() const = 0;

    /**
     * Gets the name of this command sender
     *
     * @return Name of the sender
     */
    virtual std::string_view getName() const = 0;
};

#endif // ENDSTONE_COMMAND_SENDER_H
