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
#include <cstdint>
#include <string>

namespace endstone {

/**
 * @brief Represents an IP Socket Address (hostname + port number).
 */
class SocketAddress {
public:
    SocketAddress() = default;
    SocketAddress(std::string hostname, std::uint32_t port) : hostname_(std::move(hostname)), port_(port) {}

    /**
     * Gets the hostname.
     *
     * @return The hostname of this socket address
     */
    [[nodiscard]] const std::string &getHostname() const
    {
        return hostname_;
    }

    /**
     * Gets the port number.
     *
     * @return The port number of this socket address
     */
    [[nodiscard]] std::uint32_t getPort() const
    {
        return port_;
    }

private:
    std::string hostname_;
    std::uint32_t port_;
};

}  // namespace endstone
