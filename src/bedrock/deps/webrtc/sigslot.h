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

#pragma once

#include <set>

#include "bedrock/bedrock.h"

// NOLINTBEGIN(readability-identifier-naming)
namespace sigslot {

class single_threaded {};

class _signal_base_interface;

class has_slots_interface {
private:
    using signal_connect_t = void (*)(has_slots_interface *self, _signal_base_interface *sender);
    using signal_disconnect_t = void (*)(has_slots_interface *self, _signal_base_interface *sender);
    using disconnect_all_t = void (*)(has_slots_interface *self);

    const signal_connect_t m_signal_connect;
    const signal_disconnect_t m_signal_disconnect;
    const disconnect_all_t m_disconnect_all;

protected:
    has_slots_interface(signal_connect_t conn, signal_disconnect_t disc, disconnect_all_t disc_all);
    virtual ~has_slots_interface() = default;
};

template <class mt_policy>
class has_slots : public has_slots_interface, public mt_policy {
private:
    std::set<_signal_base_interface *> m_senders;
};
BEDROCK_STATIC_ASSERT_SIZE(has_slots<single_threaded>, 48, 56);

}  // namespace sigslot
// NOLINTEND(readability-identifier-naming)
