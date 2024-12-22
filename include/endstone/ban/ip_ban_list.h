#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/ban_list.h"
#include "endstone/ban/ip_ban_entry.h"

namespace endstone {

/**
 * @brief Represents a ban list, containing banned IP addresses.
 */
class IpBanList : public BanList<IpBanEntry> {
public:
    ~IpBanList() override = default;

    /**
     * @brief Gets a BanEntry by IP address.
     *
     * @param address The IP address to search for.
     * @return IpBanEntry* The corresponding entry, or nullptr if none found.
     */
    [[nodiscard]] const IpBanEntry *getBanEntry(std::string address) const override = 0;

    /**
     * @brief Gets a BanEntry by IP address.
     *
     * @param address The IP address to search for.
     * @return IpBanEntry* The corresponding entry, or nullptr if none found.
     */
    [[nodiscard]] IpBanEntry *getBanEntry(std::string address) override = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param address The IP address of the target.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param expires The date for the ban's expiration (unban), or std::nullopt to imply forever.
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return IpBanEntry& The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    IpBanEntry &addBan(std::string address, std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                       std::optional<std::string> source) override = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param address The IP address of the target.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param duration The duration of the ban
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return IpBanEntry& The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    IpBanEntry &addBan(std::string address, std::optional<std::string> reason, std::chrono::seconds duration,
                       std::optional<std::string> source) override = 0;

    /**
     * @brief Gets a vector containing pointers to every BanEntry in this list.
     *
     * @return A vector containing pointers to every entry tracked by this list.
     */
    [[nodiscard]] std::vector<const IpBanEntry *> getEntries() const override = 0;

    /**
     * @brief Gets a vector containing pointers to every BanEntry in this list.
     *
     * @return  A vector containing pointers to every entry tracked by this list.
     */
    [[nodiscard]] std::vector<IpBanEntry *> getEntries() override = 0;

    /**
     * @brief Checks if a BanEntry exists for the target, indicating an active ban status.
     *
     * @param address The IP address to find.
     * @return true If a BanEntry exists for the target, indicating an active ban status.
     * @return false Otherwise.
     */
    [[nodiscard]] bool isBanned(std::string address) const override = 0;

    /**
     * @brief Removes the specified IP address from this list, therefore indicating a "not banned" status.
     *
     * @param address The IP address to remove from this list.
     */
    void removeBan(std::string address) override = 0;
};

}  // namespace endstone
