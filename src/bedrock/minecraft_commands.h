//
// Created by Vincent on 27/08/2023.
//

#ifndef ENDSTONE_MINECRAFT_COMMANDS_H
#define ENDSTONE_MINECRAFT_COMMANDS_H

#include "bedrock_common.h"
#include "command_context.h"

class MinecraftCommands {

public:
    struct Result {
        bool is_success;
        uint8_t category;
        uint16_t error_code;

        constexpr int getFullCode() const
        {
            return error_code | ((category | (is_success ? 0 : 0x8000)) << 16);
        }

        friend bool operator==(const Result &lhs, const Result &rhs)
        {
            return lhs.is_success == rhs.is_success && lhs.category == rhs.category && lhs.error_code == rhs.error_code;
        }

        friend bool operator!=(const Result &lhs, const Result &rhs)
        {
            return !(lhs == rhs);
        }
    };

public:
    Result *executeCommand(MinecraftCommands::Result *result, CommandContext *command_ctx, bool flag);

    static MinecraftCommands &getInstance()
    {
        return *g_instance;
    };

    inline static MinecraftCommands *g_instance{nullptr};
};

#endif // ENDSTONE_MINECRAFT_COMMANDS_H
