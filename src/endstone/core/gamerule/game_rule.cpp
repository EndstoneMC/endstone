#include "endstone/core/gamerule/game_rule.h"

#include "endstone/core/server.h"

namespace endstone::core {
EndstoneGameRule::EndstoneGameRule(std::string id, GameRules::GameRulesIndex index) : id(std::move(id)), index(index) {}
GameRuleId EndstoneGameRule::getId() const
{
    return id;
}
endstone::GameRule::Type EndstoneGameRule::getType() const
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &rules = server.getEndstoneLevel()->getHandle().getGameRules();
    ::GameRuleId rule{index};
    if (rule < 0 || rule >= rules.game_rules.size()) {
        return endstone::GameRule::Type::Invalid;
    }
    switch (rules.game_rules[rule].type) {
    case ::GameRule::Type::Invalid:
        return endstone::GameRule::Type::Invalid;
    case ::GameRule::Type::Bool:
        return endstone::GameRule::Type::Boolean;
    case ::GameRule::Type::Int:
        return endstone::GameRule::Type::Integer;
    case ::GameRule::Type::Float:
        return endstone::GameRule::Type::Float;
    }
}
}  // namespace endstone::core
