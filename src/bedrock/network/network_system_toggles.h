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

enum class TransportLayer : int {
    RakNet = 0,
    NetherWebSockets = 2,
    Unknown = 4,
    Default = 2,
};

enum class NetworkStatisticsConfig : int {
    None = 0,
    Client = 1,
    Server = 2,
};

enum class PermissionIPv6 : int {
    IPv6Denied = 0,
    IPv6Allowed = 1,
};

enum class PermissionLAN : int {
    LANDenied = 0,
    LANAllowed = 1,
};

enum class RakNetServerLANVisibility : int {
    Visible = 0,
    Hidden = 1,
};