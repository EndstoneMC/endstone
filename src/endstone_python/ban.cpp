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
#include "endstone/ban/player_ban_entry.h"
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

    py::class_<PlayerBanEntry, BanEntry>(m, "PlayerBanEntry", "Represents a ban entry for a player.")
        .def(py::init<std::string, std::optional<UUID>, std::optional<std::string>>(), py::arg("name"),
             py::arg("uuid") = std::nullopt, py::arg("xuid") = std::nullopt)
        .def_property_readonly("name", &PlayerBanEntry::getName, "Gets the banned player's name.")
        .def_property_readonly("unique_id", &PlayerBanEntry::getUniqueId,
                               "Gets the banned player's unique ID, or None if not available.")
        .def_property_readonly("xuid", &PlayerBanEntry::getXuid,
                               "Gets the banned player's Xbox user ID (XUID), or None if not available.");
}

}  // namespace endstone::detail
