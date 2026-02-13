

# File player\_ban\_list.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**player\_ban\_list.h**](player__ban__list_8h.md)

[Go to the documentation of this file](player__ban__list_8h.md)


```C++
#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/ban_list.h"
#include "endstone/ban/player_ban_entry.h"

namespace endstone {

class PlayerBanList : public BanList<PlayerBanEntry> {
public:
    [[nodiscard]] Nullable<PlayerBanEntry> getBanEntry(std::string name) const override = 0;

    [[nodiscard]] virtual Nullable<PlayerBanEntry> getBanEntry(std::string name, std::optional<UUID> uuid,
                                                               std::optional<std::string> xuid) const = 0;

    NotNull<PlayerBanEntry> addBan(std::string name, std::optional<std::string> reason,
                                   std::optional<BanEntry::Date> expires,
                                   std::optional<std::string> source) override = 0;

    virtual NotNull<PlayerBanEntry> addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                           std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                                           std::optional<std::string> source) = 0;

    NotNull<PlayerBanEntry> addBan(std::string name, std::optional<std::string> reason, std::chrono::seconds duration,
                                   std::optional<std::string> source) override = 0;

    virtual NotNull<PlayerBanEntry> addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                           std::optional<std::string> reason, std::chrono::seconds duration,
                                           std::optional<std::string> source) = 0;

    [[nodiscard]] std::vector<NotNull<PlayerBanEntry>> getEntries() const override = 0;

    [[nodiscard]] bool isBanned(std::string name) const override = 0;

    [[nodiscard]] virtual bool isBanned(std::string name, std::optional<UUID> uuid,
                                        std::optional<std::string> xuid) const = 0;

    void removeBan(std::string name) override = 0;

    virtual void removeBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid) = 0;
};

}  // namespace endstone
```


