

# File socket\_address.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**socket\_address.h**](socket__address_8h.md)

[Go to the documentation of this file](socket__address_8h.md)


```C++
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
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <string>
#include <utility>

namespace endstone {

class SocketAddress {
public:
    SocketAddress() = default;
    SocketAddress(std::string hostname, std::uint32_t port) : hostname_(std::move(hostname)), port_(port) {}

    [[nodiscard]] const std::string &getHostname() const { return hostname_; }

    [[nodiscard]] std::uint32_t getPort() const { return port_; }

    bool operator==(const SocketAddress &other) const = default;

private:
    std::string hostname_;
    std::uint32_t port_;
};

}  // namespace endstone

template <>
struct std::hash<endstone::SocketAddress> {
    std::size_t operator()(const endstone::SocketAddress &address) const noexcept
    {
        std::size_t seed = std::hash<std::string>{}(address.getHostname());
        seed ^= std::hash<std::uint32_t>{}(address.getPort()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template <>
struct std::formatter<endstone::SocketAddress> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::SocketAddress &address, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(), "{}:{}", address.getHostname(), address.getPort());
    }
};
```


