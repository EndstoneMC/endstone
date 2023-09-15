//
// Created by Vincent on 27/08/2023.
//

#pragma once

#include "bedrock_common.h"
#include "command_context.h"
#include "endstone/command/bedrock/bedrock_command.h"

struct MCRESULT {
    bool is_success;
    uint8_t category;
    uint16_t error_code;

    constexpr int getFullCode() const
    {
        return error_code | ((category | (is_success ? 0 : 0x8000)) << 16);
    }

    friend bool operator==(const MCRESULT &lhs, const MCRESULT &rhs)
    {
        return lhs.is_success == rhs.is_success && lhs.category == rhs.category && lhs.error_code == rhs.error_code;
    }

    friend bool operator!=(const MCRESULT &lhs, const MCRESULT &rhs)
    {
        return !(lhs == rhs);
    }
};

class MinecraftCommands {
public:
    BEDROCK_API MCRESULT executeCommand(CommandContext &command_ctx, bool flag) const;

private:
    inline static std::optional<std::function<bool(const std::string &, std::unique_ptr<CommandOrigin>)>>
        vanilla_dispatcher;

    // Allow the BedrockCommand::execute to use the vanilla executor
    friend bool BedrockCommand::execute(CommandSender &sender, const std::string &label,
                                        const std::vector<std::string> &args) const;
};

