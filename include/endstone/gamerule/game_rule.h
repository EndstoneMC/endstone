#pragma once
#include "endstone/detail.h"
#include "endstone/identifier.h"
#include "endstone/registry.h"
#include "endstone/server.h"
namespace endstone {
using GameRuleId = Identifier<class GameRule>;
class GameRule {
public:
    ENDSTONE_REGISTRY_TYPE(GameRule)
    enum class Type {
        Invalid,
        Boolean,
        Integer,
        Float,
    };
    [[nodiscard]] virtual GameRuleId getId() const = 0;
    [[nodiscard]] virtual Type getType() const = 0;
    virtual ~GameRule() = default;

    bool operator==(const GameRuleId &other) const
    {
        return getId() == other;
    }

    bool operator!=(const GameRuleId &other) const
    {
        return !(*this == other);
    }

    bool operator==(const GameRule &other) const
    {
        return getId() == other.getId();
    }

    bool operator!=(const GameRule &other) const
    {
        return !(*this == other);
    }

    operator GameRuleId() const
    {
        return getId();
    }
};
}  // namespace endstone
