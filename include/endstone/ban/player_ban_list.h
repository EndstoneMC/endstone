#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/ban_list.h"
#include "endstone/ban/player_ban_entry.h"

namespace endstone {

/**
 * @brief Represents a ban list, containing banned players.
 */
class PlayerBanList : public BanList<PlayerBanEntry> {
public:
    /**
     * @brief Gets a BanEntry by player name.
     *
     * @param name The player name to search for.
     * @return PlayerBanEntry The corresponding entry, or nullptr if none found.
     */
    [[nodiscard]] virtual Nullable<PlayerBanEntry> getBanEntry(std::string name) const = 0;

    /**
     * @brief Gets a BanEntry by player name, UUID, or XUID.
     *
     * @param name The player name to search for.
     * @param uuid The UUID of the player to search for, std::nullopt if not used.
     * @param xuid The XUID of the player to search for, std::nullopt if not used.
     * @return PlayerBanEntry The corresponding entry, or nullptr if none found.
     */
    [[nodiscard]] virtual Nullable<PlayerBanEntry> getBanEntry(std::string name, std::optional<UUID> uuid,
                                                               std::optional<std::string> xuid) const = 0;

    /**
     * @brief Gets a BanEntry by player name, UUID, or XUID.
     *
     * @param name The player name to search for.
     * @param uuid The UUID of the player to search for, std::nullopt if not used.
     * @param xuid The XUID of the player to search for, std::nullopt if not used.
     * @return PlayerBanEntry The corresponding entry, or nullptr if none found.
     */
    [[nodiscard]] virtual Nullable<PlayerBanEntry> getBanEntry(std::string name, std::optional<UUID> uuid,
                                                               std::optional<std::string> xuid) = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param name The name of the target.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param expires The date for the ban's expiration (unban), or std::nullopt to imply forever.
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return PlayerBanEntry The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    virtual NotNull<PlayerBanEntry> addBan(std::string name, std::optional<std::string> reason,
                                           std::optional<BanEntry::Date> expires,
                                           std::optional<std::string> source) = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param name The name of the target.
     * @param uuid The UUID of the target, std::nullopt if not used.
     * @param xuid The XUID of the target, std::nullopt if not used.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param expires The date for the ban's expiration (unban), or std::nullopt to imply forever.
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return PlayerBanEntry The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    virtual NotNull<PlayerBanEntry> addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                           std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                                           std::optional<std::string> source) = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param name The name of the target.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param duration The duration of the ban
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return PlayerBanEntry The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    virtual NotNull<PlayerBanEntry> addBan(std::string name, std::optional<std::string> reason,
                                           std::chrono::seconds duration, std::optional<std::string> source) = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param name The name of the target.
     * @param uuid The UUID of the target, std::nullopt if not used.
     * @param xuid The XUID of the target, std::nullopt if not used.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param duration The duration of the ban
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return PlayerBanEntry The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    virtual NotNull<PlayerBanEntry> addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                           std::optional<std::string> reason, std::chrono::seconds duration,
                                           std::optional<std::string> source) = 0;

    /**
     * @brief Gets a vector containing pointers to every BanEntry in this list.
     *
     * @return A vector containing pointers to every entry tracked by this list.
     */
    [[nodiscard]] virtual std::vector<NotNull<PlayerBanEntry>> getEntries() const = 0;

    /**
     * @brief Checks if a BanEntry exists for the target, indicating an active ban status.
     *
     * @param name The player name to find.
     * @return true If a BanEntry exists for the target, indicating an active ban status.
     * @return false Otherwise.
     */
    [[nodiscard]] virtual bool isBanned(std::string name) const = 0;

    /**
     * @brief Checks if a BanEntry exists for the target, indicating an active ban status.
     *
     * @param name The player name to find.
     * @param uuid The UUID of the target, std::nullopt if not used.
     * @param xuid The XUID of the target, std::nullopt if not used.
     * @return true If a BanEntry exists for the target, indicating an active ban status.
     * @return false Otherwise.
     */
    [[nodiscard]] virtual bool isBanned(std::string name, std::optional<UUID> uuid,
                                        std::optional<std::string> xuid) const = 0;

    /**
     * @brief Removes the specified player from this list, therefore indicating a "not banned" status.
     *
     * @param name The player name to remove from this list.
     */
    virtual void removeBan(std::string name) = 0;

    /**
     * @brief Removes the specified player from this list, therefore indicating a "not banned" status.
     *
     * @param name The player name to remove from this list.
     * @param uuid The unique id of the player, std::nullopt if not used.
     * @param xuid The xbox user id (xuid) of the player, std::nullopt if not used.
     */
    virtual void removeBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid) = 0;
};

}  // namespace endstone
