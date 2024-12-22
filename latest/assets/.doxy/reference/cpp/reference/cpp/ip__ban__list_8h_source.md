

# File ip\_ban\_list.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**ip\_ban\_list.h**](ip__ban__list_8h.md)

[Go to the documentation of this file](ip__ban__list_8h.md)


```C++
#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/ip_ban_entry.h"

namespace endstone {

class IpBanList {
public:
    virtual ~IpBanList() = default;

    [[nodiscard]] virtual const IpBanEntry *getBanEntry(std::string address) const = 0;

    [[nodiscard]] virtual IpBanEntry *getBanEntry(std::string address) = 0;

    virtual IpBanEntry &addBan(std::string address, std::optional<std::string> reason,
                               std::optional<BanEntry::Date> expires, std::optional<std::string> source) = 0;

    virtual IpBanEntry &addBan(std::string address, std::optional<std::string> reason, std::chrono::seconds duration,
                               std::optional<std::string> source) = 0;

    [[nodiscard]] virtual std::vector<const IpBanEntry *> getEntries() const = 0;

    [[nodiscard]] virtual std::vector<IpBanEntry *> getEntries() = 0;

    [[nodiscard]] virtual bool isBanned(std::string address) const = 0;

    virtual void removeBan(std::string address) = 0;
};

}  // namespace endstone
```


