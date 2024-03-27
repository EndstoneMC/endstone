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

#include "bedrock/event/event_coordinator.h"

#include <iomanip>
#include <sstream>

#include <spdlog/spdlog.h>

#include "endstone/detail/hook.h"

namespace {
template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

template <typename T>
std::string hexDump(T &obj)
{
    const auto *ptr = reinterpret_cast<const unsigned char *>(&obj);
    std::ostringstream oss;
    for (std::size_t i = 0; i < sizeof(obj) + 16; ++i) {
        if (i == sizeof(obj)) {
            oss << " | ";
        }

        if (i % 16 == 0) {
            oss << reinterpret_cast<const void *>(ptr + i) << ": ";
        }
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(ptr[i]) << " ";
        if (i % 16 == 15) {
            oss << "\n";
        }
    }
    oss << "\n";
    return oss.str();
}

}  // namespace

void LevelEventCoordinator::sendEvent(const EventRef<LevelGameplayEvent<void>> &ref)
{
    try {
        std::visit([](auto &&arg) { spdlog::info("Event: {}", typeid(arg).name()); }, ref.reference.event);
    }
    catch (std::bad_variant_access &e) {
        spdlog::critical("A bad variant access error has occurred. This is likely due to an update in the game. Please "
                         "report this issue on GitHub and include the following memory dump: {}",
                         hexDump(ref));
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&LevelEventCoordinator::sendEvent, this, ref);
}

void PlayerEventCoordinator::sendEvent(const EventRef<PlayerGameplayEvent<void>> &ref)
{
    try {
        std::visit([](auto &&arg) { spdlog::info("Event: {}", typeid(arg).name()); }, ref.reference.event);
    }
    catch (std::bad_variant_access &e) {
        spdlog::critical("A bad variant access error has occurred. This is likely due to an update in the game. Please "
                         "report this issue on GitHub and include the following memory dump: {}",
                         hexDump(ref));
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&PlayerEventCoordinator::sendEvent, this, ref);
}
