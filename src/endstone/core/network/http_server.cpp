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
#include "endstone/core/network/http_server.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <chrono>
#include <cstddef>
#include <format>
#include <vector>

#include <asio/as_tuple.hpp>
#include <asio/cancel_after.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/ip/v6_only.hpp>
#include <asio/steady_timer.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/write.hpp>

namespace endstone::core {

using asio::ip::tcp;
using namespace std::chrono_literals;

HttpServer::HttpServer(const std::uint16_t port, std::string bind_address)
    : port_(port), bind_address_(std::move(bind_address))
{
}

HttpServer::~HttpServer()
{
    stop();
}

std::error_code HttpServer::start()
{
    if (const auto ec = _tryCreateListeningSocket()) {
        return ec;
    }
    asio::co_spawn(io_context_, _onListenReadEvent(), asio::detached);
    thread_ = std::thread([this] { io_context_.run(); });
    return {};
}

void HttpServer::stop()
{
    io_context_.stop();
    if (thread_.joinable()) {
        thread_.join();
    }
    if (listen_socket_) {
        std::error_code ec;
        listen_socket_->close(ec);
    }
}

std::uint16_t HttpServer::port() const
{
    return port_;
}

std::error_code HttpServer::_tryCreateListeningSocket()
{
    std::error_code ec;
    std::vector<tcp::endpoint> endpoints;
    if (bind_address_.empty()) {
        endpoints = {{asio::ip::address_v6::any(), port_}, {asio::ip::address_v4::any(), port_}};
    }
    else if (const auto address = asio::ip::make_address(bind_address_, ec); !ec) {
        endpoints = {{address, port_}};
    }

    tcp::acceptor acceptor(io_context_);
    for (const auto &endpoint : endpoints) {
        acceptor.close(ec);
        acceptor.open(endpoint.protocol(), ec);
#ifndef _WIN32
        if (!ec) {
            acceptor.set_option(tcp::acceptor::reuse_address(true), ec);
        }
#endif
        if (!ec && endpoint.address().is_v6()) {
            acceptor.set_option(asio::ip::v6_only(false), ec);
        }
        if (!ec) {
            acceptor.bind(endpoint, ec);
        }
        if (!ec) {
            acceptor.listen(asio::socket_base::max_listen_connections, ec);
        }
        if (ec != asio::error::address_family_not_supported && ec != std::errc::address_not_available) {
            break;
        }
    }
    if (ec) {
        return ec;
    }
    port_ = acceptor.local_endpoint().port();
    listen_socket_.emplace(std::move(acceptor));
    return {};
}

asio::awaitable<void> HttpServer::_onListenReadEvent()
{
    while (true) {
        auto [ec, socket] = co_await listen_socket_->async_accept(asio::as_tuple(asio::use_awaitable));
        if (ec) {
            co_await asio::steady_timer(io_context_, 100ms).async_wait(asio::as_tuple(asio::use_awaitable));
            continue;
        }
        asio::co_spawn(io_context_, _serve(std::move(socket)), asio::detached);
    }
}

asio::awaitable<void> HttpServer::_serve(tcp::socket socket)
{
    constexpr std::size_t max_request_size = 64 * 1024;
    std::error_code ec;
    HttpRequest request;
    request.remote_address = socket.remote_endpoint(ec).address().to_string();

    std::string buffer;
    char chunk[4096];
    std::size_t head_end = std::string::npos;
    std::size_t total = 0;
    while (head_end == std::string::npos || buffer.size() < total) {
        const auto [error, size] = co_await socket.async_read_some(
            asio::buffer(chunk), asio::cancel_after(30s, asio::as_tuple(asio::use_awaitable)));
        if (error) {
            co_return;
        }
        buffer.append(chunk, size);
        if (buffer.starts_with("\x16\x03")) {
            constexpr char alert[] = {'\x15', '\x03', '\x01', '\x00', '\x02', '\x02', '\x28'};
            co_await asio::async_write(socket, asio::buffer(alert), asio::as_tuple(asio::use_awaitable));
            co_return;
        }
        if (head_end == std::string::npos && (head_end = buffer.find("\r\n\r\n")) != std::string::npos) {
            const std::string_view head(buffer.data(), head_end);
            const auto line = head.substr(0, head.find("\r\n"));
            const auto first_space = line.find(' ');
            const auto last_space = line.rfind(' ');
            if (first_space == std::string_view::npos || first_space == last_space) {
                co_return;
            }
            const auto target = line.substr(first_space + 1, last_space - first_space - 1);
            const auto question = target.find('?');
            request.method = line.substr(0, first_space);
            request.path = target.substr(0, question);
            request.query = question == std::string_view::npos ? "" : target.substr(question + 1);

            std::size_t length = 0;
            for (auto pos = head.find("\r\n"); pos != std::string_view::npos;) {
                const auto begin = pos + 2;
                pos = head.find("\r\n", begin);
                const auto field = head.substr(begin, pos == std::string_view::npos ? pos : pos - begin);
                const auto colon = field.find(':');
                auto value = colon == std::string_view::npos ? std::string_view{} : field.substr(colon + 1);
                const auto first = value.find_first_not_of(" \t");
                value = first == std::string_view::npos
                          ? ""
                          : value.substr(first, value.find_last_not_of(" \t") + 1 - first);
                const auto is = [&](const std::string_view name) {
                    return std::ranges::equal(field.substr(0, colon), name, [](const char a, const char b) {
                        return std::tolower(static_cast<unsigned char>(a)) == b;
                    });
                };
                if (is("content-length")) {
                    std::from_chars(value.data(), value.data() + value.size(), length);
                }
                else if (is("host")) {
                    request.host = value;
                }
            }
            total = head_end + 4 + length;
        }
        if (buffer.size() > max_request_size || total > max_request_size) {
            co_return;
        }
    }
    request.body = buffer.substr(head_end + 4, total - head_end - 4);

    const auto response = co_await onRequest(std::move(request));
    if (!response) {
        co_return;
    }
    const auto out =
        std::format("HTTP/1.1 {} {}\r\n{}Content-Length: {}\r\nConnection: close\r\n\r\n{}", response->status_code,
                    response->status_text,
                    response->content_type.empty() ? "" : std::format("Content-Type: {}\r\n", response->content_type),
                    response->body.size(), response->body);
    co_await asio::async_write(socket, asio::buffer(out), asio::as_tuple(asio::use_awaitable));
}

}  // namespace endstone::core
