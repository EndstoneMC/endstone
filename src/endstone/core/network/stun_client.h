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

#include <chrono>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace endstone::core {

/**
 * @brief Checks that @p uri names a STUN server in the form [stun:]host[:port].
 */
[[nodiscard]] bool isValidStunServer(std::string_view uri);

/**
 * @brief Asks each STUN server for this host's public IPv4 address and returns the first answer.
 *
 * Blocks for at most @p timeout, even when a DNS lookup hangs.
 */
[[nodiscard]] std::optional<std::string> queryPublicAddress(std::vector<std::string> uris,
                                                            std::chrono::milliseconds timeout);

}  // namespace endstone::core
