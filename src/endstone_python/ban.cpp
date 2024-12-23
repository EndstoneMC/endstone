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

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/ban/ban_entry.h"
#include "endstone/ban/ip_ban_entry.h"
#include "endstone/ban/ip_ban_list.h"
#include "endstone/ban/player_ban_entry.h"
#include "endstone/ban/player_ban_list.h"
#include "endstone/detail/pybind_type_caster.h"

namespace py = pybind11;

namespace endstone::detail {

void init_ban(py::module_ &m)
{
    py::class_<BanEntry>(m, "BanEntry", "A single entry from a ban list.")
        .def_property("created", &BanEntry::getCreated, &BanEntry::setCreated,
                      "Gets or sets the date this ban entry was created.")
        .def_property("source", &BanEntry::getSource, &BanEntry::setSource, "Gets or sets the source of this ban.")
        .def_property("expiration", &BanEntry::getExpiration, &BanEntry::setExpiration,
                      "Gets or sets the date this ban expires on.")
        .def_property("reason", &BanEntry::getReason, &BanEntry::setReason, "Gets or sets the reason for this ban.");

    py::class_<IpBanEntry, BanEntry>(m, "IpBanEntry", "Represents a ban entry for an IP address.")
        .def(py::init<std::string>(), py::arg("address"))
        .def_property_readonly("address", &IpBanEntry::getAddress, "Gets the banned IP address.");

    py::class_<IpBanList>(m, "IpBanList", "Represents a ban list containing banned IP addresses.")
        .def("get_ban_entry", py::overload_cast<std::string>(&IpBanList::getBanEntry),
             py::return_value_policy::reference, py::arg("address"), "Gets a BanEntry by IP address.")
        .def("add_ban",
             py::overload_cast<std::string, std::optional<std::string>, std::optional<BanEntry::Date>,
                               std::optional<std::string>>(&IpBanList::addBan),
             py::return_value_policy::reference, py::arg("address"), py::arg("reason") = std::nullopt,
             py::arg("expires") = std::nullopt, py::arg("source") = std::nullopt,
             "Adds a ban to this list, or updates an existing one.")
        .def("is_banned", py::overload_cast<std::string>(&IpBanList::isBanned, py::const_), py::arg("address"),
             "Checks if a BanEntry exists for the target by IP address.")
        .def("remove_ban", &IpBanList::removeBan, py::arg("address"), "Removes an IP address from the ban list.")
        .def_property_readonly("entries", py::overload_cast<>(&IpBanList::getEntries, py::const_),
                               py::return_value_policy::reference_internal,
                               "Gets a vector of pointers to entries in the ban list.");

    py::class_<PlayerBanEntry, BanEntry>(m, "PlayerBanEntry", "Represents a ban entry for a player.")
        .def(py::init<std::string, std::optional<UUID>, std::optional<std::string>>(), py::arg("name"),
             py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt)
        .def_property_readonly("name", &PlayerBanEntry::getName, "Gets the banned player's name.")
        .def_property_readonly("unique_id", &PlayerBanEntry::getUniqueId,
                               "Gets the banned player's unique ID, or None if not available.")
        .def_property_readonly("xuid", &PlayerBanEntry::getXuid,
                               "Gets the banned player's Xbox user ID (XUID), or None if not available.");

    py::class_<PlayerBanList>(m, "PlayerBanList", "Represents a ban list containing banned players.")
        .def("get_ban_entry",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>>(
                 &PlayerBanList::getBanEntry),
             py::return_value_policy::reference, py::arg("name"), py::arg("uuid") = std::nullopt,
             py::arg("xuid") = std::nullopt, "Gets a BanEntry by player name, UUID, or XUID.")
        .def("add_ban",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>, std::optional<std::string>,
                               std::optional<BanEntry::Date>, std::optional<std::string>>(&PlayerBanList::addBan),
             py::return_value_policy::reference, py::arg("name"), py::arg("uuid") = std::nullopt,
             py::arg("xuid") = std::nullopt, py::arg("reason") = std::nullopt, py::arg("expires") = std::nullopt,
             py::arg("source") = std::nullopt, "Adds a ban to this list, or updates an existing one.")
        .def("is_banned",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>>(&PlayerBanList::isBanned,
                                                                                             py::const_),
             py::arg("name"), py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt,
             "Checks if a BanEntry exists for the target by name, UUID, or XUID.")
        .def("remove_ban",
             py::overload_cast<std::string, std::optional<UUID>, std::optional<std::string>>(&PlayerBanList::removeBan),
             py::arg("name"), py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt,
             "Removes a player from the ban list by name, UUID, or XUID.")
        .def_property_readonly("entries", py::overload_cast<>(&PlayerBanList::getEntries, py::const_),
                               py::return_value_policy::reference_internal,
                               "Gets a vector of pointers to entries in the ban list.");
}

}  // namespace endstone::detail
