

# File ip\_ban\_list.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**ip\_ban\_list.h**](ip__ban__list_8h.md)

[Go to the documentation of this file](ip__ban__list_8h.md)


```C++
#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/ban_list.h"
#include "endstone/ban/ip_ban_entry.h"

namespace endstone {

class IpBanList : public BanList<IpBanEntry> {
public:
    ~IpBanList() override = default;

    [[nodiscard]] const IpBanEntry *getBanEntry(std::string address) const override = 0;

    [[nodiscard]] IpBanEntry *getBanEntry(std::string address) override = 0;

    IpBanEntry &addBan(std::string address, std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                       std::optional<std::string> source) override = 0;

    IpBanEntry &addBan(std::string address, std::optional<std::string> reason, std::chrono::seconds duration,
                       std::optional<std::string> source) override = 0;

    [[nodiscard]] std::vector<const IpBanEntry *> getEntries() const override = 0;

    [[nodiscard]] std::vector<IpBanEntry *> getEntries() override = 0;

    [[nodiscard]] bool isBanned(std::string address) const override = 0;

    void removeBan(std::string address) override = 0;
};

}  // namespace endstone
```


