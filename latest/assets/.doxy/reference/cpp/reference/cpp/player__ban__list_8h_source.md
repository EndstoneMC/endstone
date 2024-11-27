

# File player\_ban\_list.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**player\_ban\_list.h**](player__ban__list_8h.md)

[Go to the documentation of this file](player__ban__list_8h.md)


```C++
#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/player_ban_entry.h"

namespace endstone {

class PlayerBanList {
public:
    virtual ~PlayerBanList() = default;

    [[nodiscard]] virtual const PlayerBanEntry *getBanEntry(std::string name) const = 0;

    [[nodiscard]] virtual PlayerBanEntry *getBanEntry(std::string name) = 0;

    [[nodiscard]] virtual const PlayerBanEntry *getBanEntry(std::string name, std::optional<UUID> uuid,
                                                            std::optional<std::string> xuid) const = 0;

    [[nodiscard]] virtual PlayerBanEntry *getBanEntry(std::string name, std::optional<UUID> uuid,
                                                      std::optional<std::string> xuid) = 0;

    virtual PlayerBanEntry &addBan(std::string name, std::optional<std::string> reason,
                                   std::optional<BanEntry::Date> expires, std::optional<std::string> source) = 0;

    virtual PlayerBanEntry &addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                   std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                                   std::optional<std::string> source) = 0;

    virtual PlayerBanEntry &addBan(std::string name, std::optional<std::string> reason, std::chrono::seconds duration,
                                   std::optional<std::string> source) = 0;

    virtual PlayerBanEntry &addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                   std::optional<std::string> reason, std::chrono::seconds duration,
                                   std::optional<std::string> source) = 0;

    [[nodiscard]] virtual std::vector<const PlayerBanEntry *> getEntries() const = 0;

    [[nodiscard]] virtual std::vector<PlayerBanEntry *> getEntries() = 0;

    [[nodiscard]] virtual bool isBanned(std::string name) const = 0;

    [[nodiscard]] virtual bool isBanned(std::string name, std::optional<UUID> uuid,
                                        std::optional<std::string> xuid) const = 0;

    virtual void removeBan(std::string name) = 0;

    virtual void removeBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid) = 0;
};

}  // namespace endstone
```


