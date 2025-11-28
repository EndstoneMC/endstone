#include "bedrock/world/level/storage/game_rules.h"

#include "endstone/runtime/hook.h"

std::unique_ptr<GameRulesChangedPacket> GameRules::_setGameRule(GameRule *game_rule,
                                                                std::variant<std::monostate, bool, int, float> value,
                                                                GameRule::Type type, bool return_packet,
                                                                bool *p_value_validated, bool *p_value_changed,
                                                                GameRule::ValidationError *error_output)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&GameRules::_setGameRule, this, game_rule, value, type, return_packet,
                                       p_value_validated, p_value_changed, error_output);
}
