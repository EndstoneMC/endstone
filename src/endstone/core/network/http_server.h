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
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>

#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

namespace endstone::core {

/** @see NetherNet::HttpRequest */
struct HttpRequest {
    std::string method;
    std::string path;
    std::string query;
    std::string host;
    std::string body;
    std::string remote_address;
};

/** @see NetherNet::HttpResponse */
struct HttpResponse {
    std::uint16_t status_code{200};
    std::string_view status_text{"OK"};
    std::string_view content_type{"text/plain"};
    std::string body;

    static HttpResponse error(const std::uint16_t status_code, const std::string_view message)
    {
        return {status_code, message, "text/plain", std::string(message)};
    }
};

/** @see NetherNet::HttpServer */
class HttpServer {
public:
    HttpServer(std::uint16_t port, std::string bind_address);
    virtual ~HttpServer();

    HttpServer(const HttpServer &) = delete;
    HttpServer &operator=(const HttpServer &) = delete;

    [[nodiscard]] std::error_code start();
    void stop();
    [[nodiscard]] std::uint16_t port() const;

protected:
    /** @see NetherNet::HttpServer::onRequest */
    virtual asio::awaitable<std::optional<HttpResponse>> onRequest(HttpRequest request) = 0;

    asio::io_context io_context_;

private:
    /** @see NetherNet::HttpServer::_tryCreateListeningSocket */
    std::error_code _tryCreateListeningSocket();

    /** @see NetherNet::HttpServer::_onListenReadEvent */
    asio::awaitable<void> _onListenReadEvent();

    asio::awaitable<void> _serve(asio::ip::tcp::socket socket);

    std::uint16_t port_;
    std::string bind_address_;
    std::optional<asio::ip::tcp::acceptor> listen_socket_;
    std::thread thread_;
};

}  // namespace endstone::core
