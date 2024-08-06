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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// must be included after pybind11
#include "endstone/network/packet.h"
#include "endstone/network/packet_type.h"
#include "endstone/network/spawn_particle_effect_packet.h"

namespace py = pybind11;

namespace endstone::detail {

void init_network(py::module_ &m)
{
    py::enum_<PacketType>(m, "PacketType", "Represents the types of packets.")
        .value("SPAWN_PARTICLE_EFFECT", PacketType::SpawnParticleEffect);

    py::class_<Packet>(m, "Packet", "Represents a packet.")
        .def_property_readonly("type", &Packet::getType, "Gets the type of the packet.");

    py::class_<SpawnParticleEffectPacket, Packet>(m, "SpawnParticleEffectPacket",
                                                  "Represents a packet for spawning a particle effect.")
        .def(py::init<>())
        .def_readwrite("dimension_id", &SpawnParticleEffectPacket::dimension_id)
        .def_readwrite("actor_id", &SpawnParticleEffectPacket::actor_id)
        .def_readwrite("position", &SpawnParticleEffectPacket::position)
        .def_readwrite("effect_name", &SpawnParticleEffectPacket::effect_name)
        .def_readwrite("molang_variables_json", &SpawnParticleEffectPacket::molang_variables_json);
}

}  // namespace endstone::detail
