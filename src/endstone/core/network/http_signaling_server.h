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

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <asio/steady_timer.hpp>

#include "bedrock/deps/nethernet/events.h"
#include "bedrock/deps/nethernet/nethernet_transport.h"
#include "endstone/core/network/http_server.h"

namespace endstone::core {

/** @see NetherNet::HttpSignalingServer */
class EndstoneHttpSignalingServer : public HttpServer, public NetherNet::ISignalingInterface {
public:
    using ServerInfoCallback = std::function<std::optional<std::string>(const HttpRequest &request)>;

    EndstoneHttpSignalingServer(std::uint16_t port, std::string bind_address, ServerInfoCallback get_server_info);
    ~EndstoneHttpSignalingServer() override;

    /** @see NetherNet::HttpSignalingServer::SendSignal */
    Bedrock::Threading::Async<NetherNet::ESessionError> SendSignal(NetherNet::NetworkID from, NetherNet::NetworkID to,
                                                                   const std::string &signal) override;

    /** @see NetherNet::HttpSignalingServer::RegisterEventHandler */
    NetherNet::Subscription RegisterEventHandler(NetherNet::ISignalingEventHandler *handler) override;

protected:
    /** @see NetherNet::HttpSignalingServer::onRequest */
    asio::awaitable<std::optional<HttpResponse>> onRequest(HttpRequest request) override;

private:
    using PendingKey = std::pair<NetherNet::NetworkID, std::string>;

    struct PendingJoin {
        explicit PendingJoin(asio::io_context &io) : timer(io) {}

        asio::steady_timer timer;
        std::optional<HttpResponse> response;
        std::string candidates;
    };

    /** @see NetherNet::HttpSignalingServer::_handleJoinGET */
    std::optional<HttpResponse> _handleJoinGET(const HttpRequest &request);

    /** @see NetherNet::HttpSignalingServer::_handleJoinPOST */
    asio::awaitable<HttpResponse> _handleJoinPOST(const HttpRequest &request);

    /** @see NetherNet::HttpSignalingServer::_completePendingJoin */
    void _completePendingJoin(const NetherNet::NetworkID &remote_id, const std::string &signal);

    /** @see NetherNet::HttpSignalingServer::_timeoutPendingJoin */
    void _timeoutPendingJoin(const PendingKey &key);

    std::map<PendingKey, std::shared_ptr<PendingJoin>> pending_join_promises_;
    std::atomic<NetherNet::ISignalingEventHandler *> event_handler_{nullptr};
    ServerInfoCallback get_server_info_;
};

}  // namespace endstone::core
