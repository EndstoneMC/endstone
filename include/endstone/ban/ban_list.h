#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "endstone/ban/ban_entry.h"
#include "endstone/util/pointers.h"

namespace endstone {
/**
 * @brief Represents a ban list, containing bans.
 * @tparam T The ban target
 */
template <typename T>
class BanList {
public:
    virtual ~BanList() = default;

    /**
     * @brief Gets a BanEntry by target
     *
     * @param target The entry parameter to search for.
     * @return T* The corresponding entry, or nullptr if none found.
     */
    [[nodiscard]] virtual Nullable<T> getBanEntry(std::string target) const = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param target The target of the ban.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param expires The date for the ban's expiration (unban), or std::nullopt to imply forever.
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return T& The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    virtual NotNull<T> addBan(std::string target, std::optional<std::string> reason,
                              std::optional<BanEntry::Date> expires, std::optional<std::string> source) = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param target The target of the ban.
     * @param reason The reason for the ban, std::nullopt indicates implementation default.
     * @param duration The duration of the ban
     * @param source The source of the ban, std::nullopt indicates implementation default.
     * @return T& The entry for the newly created ban, or the entry for the (updated) previous ban.
     */
    virtual NotNull<T> addBan(std::string target, std::optional<std::string> reason, std::chrono::seconds duration,
                              std::optional<std::string> source) = 0;

    /**
     * @brief Gets a vector containing pointers to every BanEntry in this list.
     *
     * @return A vector containing pointers to every entry tracked by this list.
     */
    [[nodiscard]] virtual std::vector<NotNull<T>> getEntries() const = 0;

    /**
     * @brief Checks if a BanEntry exists for the target, indicating an active ban status.
     *
     * @param target The target to find.
     * @return true If a BanEntry exists for the target, indicating an active ban status.
     * @return false Otherwise.
     */
    [[nodiscard]] virtual bool isBanned(std::string target) const = 0;

    /**
     * @brief Removes the specified target from this list, therefore indicating a "not banned" status.
     *
     * @param target The target to remove from this list.
     */
    virtual void removeBan(std::string target) = 0;
};

}  // namespace endstone
