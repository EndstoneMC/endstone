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

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <asio/connect.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/write.hpp>
#include <gtest/gtest.h>

#include "endstone/core/network/http_signaling_server.h"

namespace endstone::core::test {

namespace {

class Handler : public NetherNet::ISignalingEventHandler {
public:
    void OnSignalingEvent(const NetherNet::SignalingEvents::MessageReceived &event) override
    {
        {
            std::lock_guard lock(mutex);
            events.push_back(event);
        }
        condition.notify_all();
    }

    NetherNet::SignalingEvents::MessageReceived wait()
    {
        std::unique_lock lock(mutex);
        EXPECT_TRUE(condition.wait_for(lock, std::chrono::seconds(5), [&] { return !events.empty(); }));
        return events.empty() ? NetherNet::SignalingEvents::MessageReceived{} : events.back();
    }

    std::mutex mutex;
    std::condition_variable condition;
    std::vector<NetherNet::SignalingEvents::MessageReceived> events;
};

NetherNet::NetworkID p2p(const std::uint64_t id)
{
    return NetherNet::NetworkID(NetherNet::P2P::NetworkID(id));
}

}  // namespace

class HttpSignalingServerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        server = std::make_unique<EndstoneHttpSignalingServer>(
            0, "", [this](const HttpRequest &request) -> std::optional<std::string> {
                if (cancel_metadata) {
                    return std::nullopt;
                }
                return request.remote_address == "127.0.0.1" ? R"({"name":"test"})" : request.remote_address;
            });
        ASSERT_FALSE(server->start());
        server->RegisterEventHandler(&handler);
    }

    std::string send(const std::string &raw)
    {
        asio::io_context io;
        asio::ip::tcp::socket socket(io);
        socket.connect({asio::ip::address_v4::loopback(), server->port()});
        std::error_code ec;
        asio::write(socket, asio::buffer(raw), ec);
        std::string reply;
        char chunk[1024];
        while (!ec) {
            reply.append(chunk, socket.read_some(asio::buffer(chunk), ec));
        }
        return reply;
    }

    std::future<std::string> post(const std::string &target, const std::string &host, const std::string &body)
    {
        return std::async(std::launch::async, [=, this] {
            return send("POST " + target + " HTTP/1.1\r\nHost: " + host +
                        "\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body);
        });
    }

    std::string session(const NetherNet::SignalingEvents::MessageReceived &event)
    {
        const auto begin = event.mMessage.find(' ') + 1;
        return event.mMessage.substr(begin, event.mMessage.find(' ', begin) - begin);
    }

    std::unique_ptr<EndstoneHttpSignalingServer> server;
    Handler handler;
    std::atomic<bool> cancel_metadata{false};
};

TEST_F(HttpSignalingServerTest, JoinGetReturnsServerInfoAndIgnoresQuery)
{
    for (const auto *target : {"/v1/join", "/v1/join?", "/v1/join?a=1&b"}) {
        const auto reply = send(std::string("GET ") + target + " HTTP/1.1\r\nHost: x\r\n\r\n");
        EXPECT_TRUE(reply.starts_with("HTTP/1.1 200 OK\r\n")) << target;
        EXPECT_NE(reply.find("Content-Type: application/json\r\n"), std::string::npos);
        EXPECT_NE(reply.find("Connection: close\r\n"), std::string::npos);
        EXPECT_TRUE(reply.ends_with(R"({"name":"test"})"));
    }
}

TEST_F(HttpSignalingServerTest, CancelledServerInfoIsClosedWithoutResponse)
{
    cancel_metadata = true;
    EXPECT_TRUE(send("GET /v1/join HTTP/1.1\r\n\r\n").empty());
}

TEST_F(HttpSignalingServerTest, UnknownRoutesAre404)
{
    for (const auto *request : {"GET /nope HTTP/1.1\r\n\r\n", "POST /v1/join HTTP/1.1\r\n\r\n",
                                "GET /v1/join/1 HTTP/1.1\r\n\r\n", "DELETE /v1/join HTTP/1.1\r\n\r\n"}) {
        const auto reply = send(request);
        EXPECT_TRUE(reply.starts_with("HTTP/1.1 404 Not Found\r\n")) << request;
        EXPECT_TRUE(reply.ends_with("\r\n\r\nNot Found")) << request;
    }
}

TEST_F(HttpSignalingServerTest, OfferValidation)
{
    const auto expect = [&](const std::string &target, const std::string &body, const std::string &message) {
        const auto reply = post(target, "x", body).get();
        EXPECT_TRUE(reply.starts_with("HTTP/1.1 400 " + message + "\r\n")) << target;
        EXPECT_TRUE(reply.ends_with("\r\n\r\n" + message)) << target;
    };
    for (const auto *target :
         {"/v1/join/abc", "/v1/join/", "/v1/join/-5", "/v1/join/12x", "/v1/join/99999999999999999999"}) {
        expect(target, "sdp", "Expected /v1/join/{networkId}");
    }
    expect("/v1/join/0", "sdp", "Invalid networkId");
    expect("/v1/join/0x0", "sdp", "Invalid networkId");
    expect("/v1/join/5", "", "Missing SDP offer in request body");
}

TEST_F(HttpSignalingServerTest, OfferIsPublishedAndAnswered)
{
    auto reply = post("/v1/join/0x10?x=1", "127.0.0.1:4242", "offer-sdp");
    const auto event = handler.wait();
    EXPECT_TRUE(event.mFrom == p2p(16));
    EXPECT_TRUE(event.mMessageId.empty());
    EXPECT_EQ(event.mMessage, "CONNECTREQUEST " + session(event) + " offer-sdp");

    const auto result = server->SendSignal(p2p(0), p2p(16), "CONNECTRESPONSE " + session(event) + " answer-sdp");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->getStatus(), Bedrock::Threading::Completed);
    EXPECT_EQ(result->getResult(), NetherNet::eSessionErrorNone);

    const auto text = reply.get();
    EXPECT_TRUE(text.starts_with("HTTP/1.1 200 OK\r\n"));
    EXPECT_NE(text.find("Content-Type: application/sdp\r\n"), std::string::npos);
    EXPECT_NE(text.find("\r\n\r\nanswer-sdp\r\na=candidate:"), std::string::npos);
    EXPECT_NE(text.find(" 127.0.0.1 4242 typ srflx raddr 0.0.0.0 rport 0\r\n"), std::string::npos);
}

TEST_F(HttpSignalingServerTest, CandidatesAreInjectedIntoAnswersOnly)
{
    auto reply = post("/v1/join/7", "[::1]", "offer-sdp");
    const auto event = handler.wait();
    server->SendSignal(p2p(0), p2p(7), "CANDIDATEADD " + session(event) + " a=candidate:x");
    EXPECT_TRUE(reply.get().ends_with("\r\n\r\na=candidate:x"));
}

TEST_F(HttpSignalingServerTest, IpV6HostUsesTheBoundPortWhenHostHasNone)
{
    auto reply = post("/v1/join/7", "[::1]", "offer-sdp");
    const auto event = handler.wait();
    server->SendSignal(p2p(0), p2p(7), "CONNECTRESPONSE " + session(event) + " ");
    EXPECT_NE(reply.get().find(" ::1 " + std::to_string(server->port()) + " typ srflx raddr :: rport 0"),
              std::string::npos);
}

TEST_F(HttpSignalingServerTest, UnknownPeerOrSessionIsDroppedSilently)
{
    auto reply = post("/v1/join/7", "x", "offer-sdp");
    const auto event = handler.wait();
    for (const auto *signal : {"CONNECTRESPONSE 0 x", "garbage", ""}) {
        EXPECT_EQ(server->SendSignal(p2p(0), p2p(7), signal)->getStatus(), Bedrock::Threading::Completed);
    }
    server->SendSignal(p2p(0), p2p(8), "CONNECTRESPONSE " + session(event) + " x");
    server->SendSignal(p2p(0), p2p(7), "CONNECTRESPONSE " + session(event) + " done");
    EXPECT_TRUE(reply.get().starts_with("HTTP/1.1 200 OK"));
}

TEST_F(HttpSignalingServerTest, TlsClientHelloGetsAnAlert)
{
    EXPECT_EQ(send(std::string("\x16\x03\x01\x02\x00", 5)), std::string("\x15\x03\x01\x00\x02\x02\x28", 7));
}

TEST_F(HttpSignalingServerTest, BadRequestsAreClosedWithoutResponse)
{
    EXPECT_TRUE(send("garbage\r\n\r\n").empty());
    EXPECT_TRUE(send("POST /v1/join/1 HTTP/1.1\r\nContent-Length: 65536\r\n\r\n").empty());
    EXPECT_TRUE(send("GET /v1/join HTTP/1.1\r\nX: " + std::string(70000, 'a') + "\r\n\r\n").empty());
}

TEST_F(HttpSignalingServerTest, StopJoinsAndRefusesConnections)
{
    const auto port = server->port();
    server->stop();
    asio::io_context io;
    asio::ip::tcp::socket socket(io);
    std::error_code ec;
    socket.connect({asio::ip::address_v4::loopback(), port}, ec);
    std::array<char, 1> byte{};
    socket.read_some(asio::buffer(byte), ec);
    EXPECT_TRUE(ec);
}

TEST_F(HttpSignalingServerTest, BindFailureIsReported)
{
    EndstoneHttpSignalingServer second(server->port(), "", {});
    EXPECT_TRUE(second.start());
    EndstoneHttpSignalingServer invalid(0, "not-an-address", {});
    EXPECT_TRUE(invalid.start());
}

}  // namespace endstone::core::test
