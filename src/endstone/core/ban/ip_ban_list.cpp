// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone/core/ban/ip_ban_list.h"

namespace endstone::core {

bool IpBanEntryMatcher::operator()(const IpBanEntry &entry, const std::string &address) const
{
    return entry.getAddress() == address;
}

const IpBanEntry *EndstoneIpBanList::getBanEntry(std::string address) const
{
    return EndstoneBanList::getBanEntry(address);
}

IpBanEntry *EndstoneIpBanList::getBanEntry(std::string address)
{
    return EndstoneBanList::getBanEntry(address);
}

IpBanEntry &EndstoneIpBanList::addBan(std::string address, std::optional<std::string> reason,
                                      std::optional<BanEntry::Date> expires, std::optional<std::string> source)
{
    return EndstoneBanList::addBan(address, reason, expires, source);
}

IpBanEntry &EndstoneIpBanList::addBan(std::string address, std::optional<std::string> reason,
                                      std::chrono::seconds duration, std::optional<std::string> source)
{
    return EndstoneBanList::addBan(address, reason, duration, source);
}

std::vector<const IpBanEntry *> EndstoneIpBanList::getEntries() const
{
    return EndstoneBanList::getEntries();
}

std::vector<IpBanEntry *> EndstoneIpBanList::getEntries()
{
    return EndstoneBanList::getEntries();
}

bool EndstoneIpBanList::isBanned(std::string address) const
{
    return EndstoneBanList::isBanned(address);
}

void EndstoneIpBanList::removeBan(std::string address)
{
    EndstoneBanList::removeBan(address);
}

}  // namespace endstone::core
