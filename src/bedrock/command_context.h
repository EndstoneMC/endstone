//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_COMMAND_CONTEXT_H
#define ENDSTONE_COMMAND_CONTEXT_H

#include "bedrock_common.h"
#include "command_origin.h"

class CommandContext {

public:
    CommandContext(const std::string &command_line, std::unique_ptr<CommandOrigin> sender, int command_version)
        : command_line_(command_line), origin_(std::move(sender)), command_version_(command_version)
    {
    }

    const std::string &getCommandLine() const
    {
        return command_line_;
    }

    void setCommandLine(const std::string &commandLine)
    {
        command_line_ = commandLine;
    }

    const std::unique_ptr<CommandOrigin> &getOrigin() const
    {
        return origin_;
    }

    std::unique_ptr<CommandOrigin> takeOrigin()
    {
        return std::move(origin_);
    }

    int getCommandVersion() const
    {
        return command_version_;
    }

private:
    std::string command_line_;              // +0
    std::unique_ptr<CommandOrigin> origin_; // +32
    int command_version_;                   // +40
};

static_assert(sizeof(CommandContext) == 48);

#endif // ENDSTONE_COMMAND_CONTEXT_H
