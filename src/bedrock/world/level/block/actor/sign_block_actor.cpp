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

#include "bedrock/world/level/block/actor/sign_block_actor.h"

#include <utility>

#include "bedrock/symbol.h"

const std::string &SignBlockActor::Text::getMessage() const
{
    if (text_object_message_.isEmpty()) {
        return message_;
    }
    const_cast<Text *>(this)->text_object_string_ = text_object_message_.asString();
    return text_object_string_;
}

void SignBlockActor::Text::setMessage(std::string message)
{
    message_ = std::move(message);
    text_object_message_.clear();
    cached_message_.dirty = true;
}

SignTextSide SignBlockActor::getSideFacingPlayer(const Player &player) const
{
    return BEDROCK_CALL(&SignBlockActor::getSideFacingPlayer, this, player);
}

void SignBlockActor::setMessageForServerScripingOnly(SignTextSide side, std::string message, std::string owner_id)
{
    auto &text = _getText(side);
    text.setMessage(std::move(message));
    text.setEditedBy(owner_id);
    setChanged();
}
