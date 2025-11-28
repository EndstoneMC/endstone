#pragma once

#include "bedrock/world/level/storage/game_rules.h"
#include "endstone/gamerule/game_rule.h"

namespace endstone::core {
class EndstoneGameRule : public GameRule {
public:
    EndstoneGameRule(std::string id, GameRules::GameRulesIndex index);
    [[nodiscard]] GameRuleId getId() const override;
    [[nodiscard]] Type getType() const override;
    std::string id;
    GameRules::GameRulesIndex index;
};
}  // namespace endstone::core
