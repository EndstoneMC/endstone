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
#include "endstone/core/network/stun_client.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <random>
#include <string_view>
#include <system_error>
#include <thread>
#include <utility>

#include <asio/buffer.hpp>
#include <asio/error.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/ip/udp.hpp>
#include <asio/steady_timer.hpp>

namespace endstone::core {

namespace {

using asio::ip::udp;

constexpr std::string_view StunScheme = "stun:";
constexpr std::uint16_t DefaultStunPort = 3478;
constexpr std::uint16_t BindingRequest = 0x0001;
constexpr std::uint16_t BindingSuccessResponse = 0x0101;
constexpr std::uint16_t MappedAddress = 0x0001;
constexpr std::uint16_t XorMappedAddress = 0x0020;
constexpr std::uint8_t FamilyIpv4 = 0x01;
constexpr std::uint32_t MagicCookie = 0x2112A442;
constexpr std::size_t HeaderSize = 20;
constexpr std::size_t MaxResponseSize = 1500;
constexpr auto RetransmitInterval = std::chrono::milliseconds(500);

using TransactionId = std::array<std::uint8_t, 12>;

std::uint16_t readU16(const std::uint8_t *data)
{
    return static_cast<std::uint16_t>((data[0] << 8) | data[1]);
}

std::uint32_t readU32(const std::uint8_t *data)
{
    return (std::uint32_t{data[0]} << 24) | (std::uint32_t{data[1]} << 16) | (std::uint32_t{data[2]} << 8) |
           std::uint32_t{data[3]};
}

void writeU16(std::uint8_t *data, const std::uint16_t value)
{
    data[0] = static_cast<std::uint8_t>(value >> 8);
    data[1] = static_cast<std::uint8_t>(value);
}

void writeU32(std::uint8_t *data, const std::uint32_t value)
{
    data[0] = static_cast<std::uint8_t>(value >> 24);
    data[1] = static_cast<std::uint8_t>(value >> 16);
    data[2] = static_cast<std::uint8_t>(value >> 8);
    data[3] = static_cast<std::uint8_t>(value);
}

// [stun:]host[:port]
std::optional<std::pair<std::string, std::uint16_t>> parseUri(std::string_view uri)
{
    if (uri.starts_with(StunScheme)) {
        uri.remove_prefix(StunScheme.size());
    }
    auto host = uri;
    auto port = DefaultStunPort;
    if (const auto colon = uri.rfind(':'); colon != std::string_view::npos) {
        host = uri.substr(0, colon);
        const auto text = uri.substr(colon + 1);
        const auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), port);
        if (ec != std::errc{} || ptr != text.data() + text.size() || port == 0) {
            return std::nullopt;
        }
    }
    if (host.empty() || host.find_first_of(":/@?") != std::string_view::npos) {
        return std::nullopt;
    }
    return std::pair{std::string{host}, port};
}

std::vector<udp::endpoint> resolve(udp::resolver &resolver, const std::vector<std::string> &uris)
{
    std::vector<udp::endpoint> servers;
    for (const auto &uri : uris) {
        const auto target = parseUri(uri);
        if (!target) {
            continue;
        }
        std::error_code error;
        const auto results = resolver.resolve(udp::v4(), target->first, "", error);
        if (!error && !results.empty()) {
            auto endpoint = results.begin()->endpoint();
            endpoint.port(target->second);
            servers.push_back(endpoint);
        }
    }
    return servers;
}

std::array<std::uint8_t, HeaderSize> makeBindingRequest(const TransactionId &id)
{
    std::array<std::uint8_t, HeaderSize> message{};
    writeU16(message.data(), BindingRequest);
    writeU32(message.data() + 4, MagicCookie);
    std::ranges::copy(id, message.begin() + 8);
    return message;
}

std::optional<std::string> parseBindingResponse(const std::uint8_t *data, const std::size_t size,
                                                const TransactionId &id)
{
    if (size < HeaderSize || readU16(data) != BindingSuccessResponse || readU32(data + 4) != MagicCookie ||
        !std::equal(id.begin(), id.end(), data + 8)) {
        return std::nullopt;
    }

    const auto end = HeaderSize + std::min<std::size_t>(readU16(data + 2), size - HeaderSize);
    std::optional<std::uint32_t> mapped;
    for (auto offset = HeaderSize; offset + 4 <= end;) {
        const auto type = readU16(data + offset);
        const auto length = readU16(data + offset + 2);
        const auto *value = data + offset + 4;
        if (offset + 4 + length > end) {
            break;
        }
        if ((type == XorMappedAddress || type == MappedAddress) && length >= 8 && value[1] == FamilyIpv4) {
            mapped = type == XorMappedAddress ? readU32(value + 4) ^ MagicCookie : readU32(value + 4);
            if (type == XorMappedAddress) {
                break;
            }
        }
        offset += 4 + ((length + 3) & ~std::size_t{3});
    }
    if (!mapped) {
        return std::nullopt;
    }
    return asio::ip::address_v4(*mapped).to_string();
}

std::optional<std::string> query(const std::vector<std::string> &uris,
                                 const std::chrono::steady_clock::time_point deadline)
{
    asio::io_context io;
    udp::resolver resolver(io);
    const auto servers = resolve(resolver, uris);
    if (servers.empty()) {
        return std::nullopt;
    }

    TransactionId id{};
    std::random_device random;
    for (auto &byte : id) {
        byte = static_cast<std::uint8_t>(random());
    }
    const auto request = makeBindingRequest(id);

    std::optional<std::string> result;
    std::array<std::uint8_t, MaxResponseSize> buffer{};
    udp::endpoint sender;
    udp::socket socket(io);
    std::error_code error;
    socket.open(udp::v4(), error);
    if (!error) {
        socket.bind(udp::endpoint(udp::v4(), 0), error);
    }
    if (error) {
        return std::nullopt;
    }

    asio::steady_timer timer(io);
    std::function<void()> send = [&] {
        for (const auto &server : servers) {
            std::error_code ignored;
            socket.send_to(asio::buffer(request), server, 0, ignored);
        }
        timer.expires_after(RetransmitInterval);
        timer.async_wait([&](const std::error_code &ec) {
            if (!ec) {
                send();
            }
        });
    };
    std::function<void()> receive = [&] {
        socket.async_receive_from(asio::buffer(buffer), sender, [&](const std::error_code &ec, const std::size_t size) {
            if (ec == asio::error::operation_aborted) {
                return;
            }
            if (!ec) {
                result = parseBindingResponse(buffer.data(), size, id);
            }
            if (result) {
                io.stop();
                return;
            }
            receive();
        });
    };

    send();
    receive();
    io.run_until(deadline);
    return result;
}

}  // namespace

bool isValidStunServer(const std::string_view uri)
{
    return parseUri(uri).has_value();
}

std::optional<std::string> queryPublicAddress(std::vector<std::string> uris, const std::chrono::milliseconds timeout)
{
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    auto promise = std::make_shared<std::promise<std::optional<std::string>>>();
    auto future = promise->get_future();
    try {
        std::thread([uris = std::move(uris), promise, deadline] {
            try {
                promise->set_value(query(uris, deadline));
            }
            catch (...) {
                promise->set_value(std::nullopt);
            }
        }).detach();
    }
    catch (const std::system_error &) {
        return std::nullopt;
    }
    if (future.wait_until(deadline) != std::future_status::ready) {
        return std::nullopt;
    }
    return future.get();
}

}  // namespace endstone::core
