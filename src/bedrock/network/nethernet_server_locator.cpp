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

#include "bedrock/network/nethernet_server_locator.h"

#include <system_error>
#include <utility>

void NetherNetServerLocator::ServerData::write(BinaryStream &stream) const
{
    stream.writeByte(VERSION_NUMBER, "dataVersion", nullptr);
    stream.writeString(server_name, "name", nullptr);
    stream.writeVarInt(protocol, "protocol", nullptr);
    stream.writeString(version, "version", nullptr);
    stream.writeString(level_name, "level", nullptr);
    stream.writeVarInt(num_players, "players", nullptr);
    stream.writeVarInt(max_num_players, "maxPlayers", nullptr);
    stream.writeVarInt(static_cast<int>(game_type), "gameType", nullptr);
    stream.writeBool(is_editor_world, "editor", nullptr);
    stream.writeBool(is_hardcore, "hardcore", nullptr);
    stream.writeBool(supported_auth.accepts_online_auth, "onlineAuth", nullptr);
    stream.writeBool(supported_auth.accepts_self_signed_auth, "selfSignedAuth", nullptr);
    stream.writeString(nonce.value, "nonce", nullptr);
    stream.writeVarInt(static_cast<int>(connection_type), "connection", nullptr);
}

Bedrock::Result<void> NetherNetServerLocator::ServerData::read(ReadOnlyBinaryStream &stream)
{
    auto data_version = stream.getByte();
    if (!data_version.ignoreError()) {
        return BEDROCK_RETHROW(data_version);
    }
    if (data_version.discardError().value() != VERSION_NUMBER) {
        return BEDROCK_NEW_ERROR(std::errc::not_supported);
    }

    auto name_result = stream.getString(stream.getLength());
    if (!name_result.ignoreError()) {
        return BEDROCK_RETHROW(name_result);
    }
    server_name = std::move(name_result.discardError().value());

    auto protocol_result = stream.getVarInt64();
    if (!protocol_result.ignoreError()) {
        return BEDROCK_RETHROW(protocol_result);
    }
    protocol = static_cast<int>(protocol_result.discardError().value());

    auto version_result = stream.getString(stream.getLength());
    if (!version_result.ignoreError()) {
        return BEDROCK_RETHROW(version_result);
    }
    version = std::move(version_result.discardError().value());

    auto level_result = stream.getString(stream.getLength());
    if (!level_result.ignoreError()) {
        return BEDROCK_RETHROW(level_result);
    }
    level_name = std::move(level_result.discardError().value());

    auto players_result = stream.getVarInt64();
    if (!players_result.ignoreError()) {
        return BEDROCK_RETHROW(players_result);
    }
    num_players = static_cast<int>(players_result.discardError().value());

    auto max_players_result = stream.getVarInt64();
    if (!max_players_result.ignoreError()) {
        return BEDROCK_RETHROW(max_players_result);
    }
    max_num_players = static_cast<int>(max_players_result.discardError().value());

    auto game_type_result = stream.getVarInt64();
    if (!game_type_result.ignoreError()) {
        return BEDROCK_RETHROW(game_type_result);
    }
    game_type = static_cast<GameType>(game_type_result.discardError().value());

    auto editor_result = stream.getBool();
    if (!editor_result.ignoreError()) {
        return BEDROCK_RETHROW(editor_result);
    }
    is_editor_world = editor_result.discardError().value();

    auto hardcore_result = stream.getBool();
    if (!hardcore_result.ignoreError()) {
        return BEDROCK_RETHROW(hardcore_result);
    }
    is_hardcore = hardcore_result.discardError().value();

    auto online_auth_result = stream.getBool();
    if (!online_auth_result.ignoreError()) {
        return BEDROCK_RETHROW(online_auth_result);
    }
    supported_auth.accepts_online_auth = online_auth_result.discardError().value();

    auto self_signed_auth_result = stream.getBool();
    if (!self_signed_auth_result.ignoreError()) {
        return BEDROCK_RETHROW(self_signed_auth_result);
    }
    supported_auth.accepts_self_signed_auth = self_signed_auth_result.discardError().value();

    auto nonce_result = stream.getString(stream.getLength());
    if (!nonce_result.ignoreError()) {
        return BEDROCK_RETHROW(nonce_result);
    }
    nonce.value = std::move(nonce_result.discardError().value());

    auto connection_result = stream.getVarInt64();
    if (!connection_result.ignoreError()) {
        return BEDROCK_RETHROW(connection_result);
    }
    connection_type = static_cast<Social::ConnectionType>(connection_result.discardError().value());
    return {};
}
