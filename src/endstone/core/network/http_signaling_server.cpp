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
#include "endstone/core/network/http_signaling_server.h"

#include <cerrno>
#include <charconv>
#include <chrono>
#include <cstdlib>
#include <format>

#include <asio/as_tuple.hpp>
#include <asio/cancel_after.hpp>
#include <asio/post.hpp>
#include <asio/use_awaitable.hpp>

#include "endstone/core/logger_factory.h"

namespace endstone::core {

using namespace std::chrono_literals;

namespace {

class CompletedSessionResult final : public Bedrock::Threading::IAsyncResult<NetherNet::ESessionError> {
public:
    [[nodiscard]] Bedrock::Threading::AsyncStatus getStatus() const override { return Bedrock::Threading::Completed; }

    [[nodiscard]] std::error_code getError() const override { return {}; }

    NetherNet::ESessionError getResult() const override { return NetherNet::eSessionErrorNone; }

    void cancel() override {}

    void addOnComplete(CompletionHandler handler) override
    {
        if (handler) {
            handler(*this);
        }
    }
};

}  // namespace

EndstoneHttpSignalingServer::EndstoneHttpSignalingServer(const std::uint16_t port, std::string bind_address,
                                                         ServerInfoCallback get_server_info)
    : HttpServer(port, std::move(bind_address)), get_server_info_(std::move(get_server_info))
{
}

EndstoneHttpSignalingServer::~EndstoneHttpSignalingServer()
{
    stop();
}

NetherNet::Subscription EndstoneHttpSignalingServer::RegisterEventHandler(NetherNet::ISignalingEventHandler *handler)
{
    event_handler_ = handler;
    return {};
}

Bedrock::Threading::Async<NetherNet::ESessionError> EndstoneHttpSignalingServer::SendSignal(
    NetherNet::NetworkID, const NetherNet::NetworkID to, const std::string &signal)
{
    asio::post(io_context_, [this, to, signal] { _completePendingJoin(to, signal); });
    Bedrock::Threading::Async<NetherNet::ESessionError> async;
    static_cast<std::shared_ptr<Bedrock::Threading::IAsyncResult<NetherNet::ESessionError>> &>(async) =
        std::make_shared<CompletedSessionResult>();
    return async;
}

asio::awaitable<std::optional<HttpResponse>> EndstoneHttpSignalingServer::onRequest(HttpRequest request)
{
    if (request.method == "GET" && request.path == "/v1/join") {
        co_return _handleJoinGET(request);
    }
    if (request.method == "POST" && request.path.starts_with("/v1/join/")) {
        co_return co_await _handleJoinPOST(request);
    }
    co_return HttpResponse::error(404, "Not Found");
}

std::optional<HttpResponse> EndstoneHttpSignalingServer::_handleJoinGET(const HttpRequest &request)
{
    auto json = get_server_info_(request);
    if (!json) {
        return std::nullopt;
    }
    return HttpResponse{200, "OK", "application/json", std::move(*json)};
}

asio::awaitable<HttpResponse> EndstoneHttpSignalingServer::_handleJoinPOST(const HttpRequest &request)
{
    const auto text = request.path.substr(std::string_view("/v1/join/").size());
    const auto first = text.find_first_not_of(" \t\n\v\f\r");
    errno = 0;
    char *end = nullptr;
    const auto remote_id = std::strtoull(text.c_str(), &end, 0);
    if (text.empty() || text.size() > 127 || first == std::string::npos || text[first] == '-' || errno == ERANGE ||
        end != text.c_str() + text.size()) {
        co_return HttpResponse::error(400, "Expected /v1/join/{networkId}");
    }
    if (remote_id == 0) {
        co_return HttpResponse::error(400, "Invalid networkId");
    }
    if (request.body.empty()) {
        co_return HttpResponse::error(400, "Missing SDP offer in request body");
    }

    auto join = std::make_shared<PendingJoin>(io_context_);
    std::string_view host = request.host;
    std::string_view port_text;
    if (host.starts_with('[')) {
        const auto close = host.find(']');
        port_text = close == std::string_view::npos ? "" : host.substr(close + 1);
        host = host.substr(1, close == std::string_view::npos ? close : close - 1);
        port_text.remove_prefix(port_text.empty() ? 0 : 1);
    }
    else if (const auto colon = host.find(':'); colon != std::string_view::npos && colon == host.rfind(':')) {
        port_text = host.substr(colon + 1);
        host = host.substr(0, colon);
    }
    std::uint16_t candidate_port = 0;
    std::from_chars(port_text.data(), port_text.data() + port_text.size(), candidate_port);
    if (candidate_port == 0) {
        candidate_port = port();
    }

    std::vector<asio::ip::address> addresses;
    std::error_code ec;
    if (const auto literal = asio::ip::make_address(host, ec); !ec) {
        addresses.push_back(literal);
    }
    else if (!host.empty()) {
        asio::ip::tcp::resolver resolver(io_context_);
        const auto [error, results] =
            co_await resolver.async_resolve(std::string(host), "0", asio::ip::tcp::resolver::numeric_service,
                                            asio::cancel_after(2s, asio::as_tuple(asio::use_awaitable)));
        for (const auto &result : results) {
            if (!result.endpoint().address().is_unspecified() && !result.endpoint().address().is_multicast()) {
                addresses.push_back(result.endpoint().address());
            }
        }
    }
    for (std::size_t i = 0; i < addresses.size(); ++i) {
        join->candidates +=
            std::format("a=candidate:{} 1 udp {} {} {} typ srflx raddr {} rport 0\r\n", 1000 + i, 1677721855 - i,
                        addresses[i].to_string(), candidate_port, addresses[i].is_v6() ? "::" : "0.0.0.0");
    }

    static std::atomic<std::uint64_t> next_session_id{1};
    const PendingKey key{NetherNet::NetworkID(NetherNet::P2P::NetworkID(remote_id)),
                         std::to_string(next_session_id.fetch_add(1))};
    pending_join_promises_.emplace(key, join);

    if (auto *handler = event_handler_.load()) {
        NetherNet::SignalingEvents::MessageReceived event;
        event.mFrom = key.first;
        event.mMessage = std::format("CONNECTREQUEST {} {}", key.second, request.body);
        handler->OnSignalingEvent(event);
    }

    join->timer.expires_after(10s);
    co_await join->timer.async_wait(asio::as_tuple(asio::use_awaitable));
    if (!join->response) {
        _timeoutPendingJoin(key);
        co_return HttpResponse::error(504, "Gateway Timeout");
    }
    co_return std::move(*join->response);
}

void EndstoneHttpSignalingServer::_completePendingJoin(const NetherNet::NetworkID &remote_id, const std::string &signal)
{
    const auto first = signal.find(' ');
    const auto second = signal.find(' ', first + 1);
    if (first == std::string::npos) {
        return;
    }
    const auto it = pending_join_promises_.find({remote_id, signal.substr(first + 1, second - first - 1)});
    if (it == pending_join_promises_.end()) {
        return;
    }
    const auto join = it->second;
    pending_join_promises_.erase(it);

    auto payload = second == std::string::npos ? std::string{} : signal.substr(second + 1);
    if (signal.starts_with("CONNECTRESPONSE ")) {
        payload += payload.empty() || payload.back() == '\n' ? "" : "\r\n";
        payload += join->candidates;
    }
    join->response = HttpResponse{200, "OK", "application/sdp", std::move(payload)};
    join->timer.cancel();
}

void EndstoneHttpSignalingServer::_timeoutPendingJoin(const PendingKey &key)
{
    if (pending_join_promises_.erase(key) > 0) {
        LoggerFactory::getLogger("HttpSignalingServer")
            .warning("Timed out waiting for SDP answer for peer {} session {}", key.first.toString(), key.second);
    }
}

}  // namespace endstone::core
