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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_ban(py::module_ &m)
{
    py::class_<BanEntry, py::smart_holder>(m, "BanEntry", "A single entry from a ban list.")
        .def_property("created", &BanEntry::getCreated, &BanEntry::setCreated,
                      "The date this ban entry was created.")
        .def_property("source", &BanEntry::getSource, &BanEntry::setSource, "The source of this ban.")
        .def_property("expiration", &BanEntry::getExpiration, &BanEntry::setExpiration,
                      "The date this ban expires on, or None for no defined end date.")
        .def_property("reason", &BanEntry::getReason, &BanEntry::setReason, "The reason for this ban.");

    py::class_<IpBanEntry, BanEntry, py::smart_holder>(m, "IpBanEntry", "Represents an entry for a banned IP address.")
        .def(py::init<std::string>(), py::arg("address"))
        .def_property_readonly("address", &IpBanEntry::getAddress, "The banned IP address.");

    py::class_<IpBanList>(m, "IpBanList", "Represents a ban list, containing banned IP addresses.")
        .def("get_ban_entry", &IpBanList::getBanEntry, py::arg("address"), R"doc(
    Gets a BanEntry by IP address.

    Args:
        address: The IP address to search for.

    Returns:
        The corresponding entry, or None if none found.
)doc")
        .def("add_ban",
             py::overload_cast<std::string, std::optional<std::string>, std::optional<BanEntry::Date>,
                               std::optional<std::string>>(&IpBanList::addBan),
             py::arg("address"), py::arg("reason") = std::nullopt, py::arg("expires") = std::nullopt,
             py::arg("source") = std::nullopt, R"doc(
    Adds a ban to this list. If a previous ban exists, this will update the previous entry.

    Args:
        address: The IP address of the target.
        reason: The reason for the ban, None indicates implementation default.
        expires: The date for the ban's expiration (unban), or None to imply forever.
        source: The source of the ban, None indicates implementation default.

    Returns:
        The entry for the newly created ban, or the entry for the (updated) previous ban.
)doc")
        .def("is_banned", &IpBanList::isBanned, py::arg("address"), R"doc(
    Checks if a BanEntry exists for the target, indicating an active ban status.

    Args:
        address: The IP address to find.

    Returns:
        True if a BanEntry exists for the target, indicating an active ban status, False otherwise.
)doc")
        .def("remove_ban", &IpBanList::removeBan, py::arg("address"), R"doc(
    Removes the specified IP address from this list, therefore indicating a "not banned" status.

    Args:
        address: The IP address to remove from this list.
)doc")
        .def_property_readonly("entries", &IpBanList::getEntries,
                               "A vector containing pointers to every entry tracked by this list.");

    py::class_<PlayerBanEntry, BanEntry, py::smart_holder>(m, "PlayerBanEntry", "Represents an entry for a banned player.")
        .def(py::init<std::string, std::optional<UUID>, std::optional<std::string>>(), py::arg("name"),
             py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt)
        .def_property_readonly("name", &PlayerBanEntry::getName, "The banned player's name.")
        .def_property_readonly("unique_id", &PlayerBanEntry::getUniqueId,
                               "The banned player's unique id, or None if not available.")
        .def_property_readonly("xuid", &PlayerBanEntry::getXuid,
                               "The banned player's xbox user id (xuid), or None if not available.");

    py::class_<PlayerBanList>(m, "PlayerBanList", "Represents a ban list, containing banned players.")
        .def("get_ban_entry",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>>(
                 &PlayerBanList::getBanEntry, py::const_),
             py::arg("name"), py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt, R"doc(
    Gets a BanEntry by player name, UUID, or XUID.

    Args:
        name: The player name to search for.
        uuid: The UUID of the player to search for, None if not used.
        xuid: The XUID of the player to search for, None if not used.

    Returns:
        The corresponding entry, or None if none found.
)doc")
        .def("add_ban",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>, std::optional<std::string>,
                               std::optional<BanEntry::Date>, std::optional<std::string>>(&PlayerBanList::addBan),
             py::arg("name"), py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt,
             py::arg("reason") = std::nullopt, py::arg("expires") = std::nullopt, py::arg("source") = std::nullopt,
             R"doc(
    Adds a ban to this list. If a previous ban exists, this will update the previous entry.

    Args:
        name: The name of the target.
        uuid: The UUID of the target, None if not used.
        xuid: The XUID of the target, None if not used.
        reason: The reason for the ban, None indicates implementation default.
        expires: The date for the ban's expiration (unban), or None to imply forever.
        source: The source of the ban, None indicates implementation default.

    Returns:
        The entry for the newly created ban, or the entry for the (updated) previous ban.
)doc")
        .def("is_banned",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>>(&PlayerBanList::isBanned,
                                                                                             py::const_),
             py::arg("name"), py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt, R"doc(
    Checks if a BanEntry exists for the target, indicating an active ban status.

    Args:
        name: The player name to find.
        uuid: The UUID of the target, None if not used.
        xuid: The XUID of the target, None if not used.

    Returns:
        True if a BanEntry exists for the target, indicating an active ban status, False otherwise.
)doc")
        .def("remove_ban",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>>(&PlayerBanList::removeBan),
             py::arg("name"), py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt, R"doc(
    Removes the specified player from this list, therefore indicating a "not banned" status.

    Args:
        name: The player name to remove from this list.
        uuid: The unique id of the player, None if not used.
        xuid: The xbox user id (xuid) of the player, None if not used.
)doc")
        .def_property_readonly("entries", &PlayerBanList::getEntries,
                               "A vector containing pointers to every entry tracked by this list.");
}

}  // namespace endstone::python
