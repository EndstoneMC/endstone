

# File ban\_list.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**ban\_list.h**](ban__list_8h.md)

[Go to the documentation of this file](ban__list_8h.md)


```C++
#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/ban_entry.h"

namespace endstone {

template <typename T>
class BanList {
public:
    virtual ~BanList() = default;

    [[nodiscard]] virtual const T *getBanEntry(std::string target) const = 0;

    [[nodiscard]] virtual T *getBanEntry(std::string target) = 0;

    virtual T &addBan(std::string target, std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                      std::optional<std::string> source) = 0;

    virtual T &addBan(std::string target, std::optional<std::string> reason, std::chrono::seconds duration,
                      std::optional<std::string> source) = 0;

    [[nodiscard]] virtual std::vector<const T *> getEntries() const = 0;

    [[nodiscard]] virtual std::vector<T *> getEntries() = 0;

    [[nodiscard]] virtual bool isBanned(std::string target) const = 0;

    virtual void removeBan(std::string target) = 0;
};

}  // namespace endstone
```


