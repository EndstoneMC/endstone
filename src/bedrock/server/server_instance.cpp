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

#include "bedrock/server/server_instance.h"

Minecraft *ServerInstance::getMinecraft()
{
    return minecraft_.get();
}

ServerNetworkSystem &ServerInstance::getNetwork()
{
    return *network_;
}

PacketSender &ServerInstance::getPacketSender()
{
    return *packet_sender_;
}

Bedrock::NonOwnerPointer<CDNConfig> ServerInstance::getCDNConfig() const
{
    return cdn_config_;
}

Bedrock::NonOwnerPointer<ServerTextSettings> ServerInstance::getServerTextSettings() const
{
    return server_text_settings_;
}
