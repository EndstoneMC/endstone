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

#include <cstddef>
#include <memory>
#include <optional>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/color.h"
#include "bedrock/input/input_mode.h"
#include "bedrock/network/packet/open_sign_packet.h"
#include "bedrock/platform/brstd/move_only_function.h"
#include "bedrock/textobject/text_object.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"

enum class SignTextSide : int {
    Front = 0,
    Back = 1,
};

class SignBlockActor : public VanillaBlockActor {
public:
    static constexpr int NUM_LINES = 4;

    struct CachedLineData {
        std::string text;
        int line_length;
    };

    struct CachedMessageData {
        CachedLineData line_data[NUM_LINES];
        unsigned int num_lines;
        std::string filtered_message;
        const void *cached_font_compare;
        bool dirty;
        InputMode input_mode;
        NewInteractionModel interaction_model;
    };

    class Text {
    public:
        static constexpr std::size_t MAX_TEXT_LENGTH = 512;

        [[nodiscard]] const std::string &getMessage() const;
        void setMessage(std::string message);
        void setEditedBy(const std::string &player_xuid) { last_edit_xuid_ = player_xuid; }
        [[nodiscard]] const mce::Color &getColor() const { return sign_text_color_; }
        void setColor(const mce::Color &color) { sign_text_color_ = color; }
        [[nodiscard]] bool isGlowing() const { return glowing_; }
        void setGlowing(bool glowing) { glowing_ = glowing; }
        [[nodiscard]] bool getHideGlowOutline() const { return hide_glow_outline_; }
        void setHideGlowOutline(bool hide_glow_outline) { hide_glow_outline_ = hide_glow_outline; }
        [[nodiscard]] bool shouldPersistFormatting() const { return should_persist_formatting_; }

    private:
        std::string message_;
        std::string filtered_message_;
        TextObjectRoot text_object_message_;
        CachedMessageData cached_message_;
        mce::Color sign_text_color_;
        bool glowing_;
        bool hide_glow_outline_;
        bool should_persist_formatting_;
        std::string last_edit_xuid_;
        std::string text_object_string_;
    };

    static constexpr BlockActorType TypeId = BlockActorType::Sign;

    [[nodiscard]] const std::string &getMessage(SignTextSide side) const { return _getText(side).getMessage(); }
    [[nodiscard]] const mce::Color &getSignTextColor(SignTextSide side) const { return _getText(side).getColor(); }
    void setSignTextColor(SignTextSide side, const mce::Color &color) { _getText(side).setColor(color); }
    [[nodiscard]] bool getIsGlowing(SignTextSide side) const { return _getText(side).isGlowing(); }
    void setIsGlowing(SignTextSide side, bool is_glowing) { _getText(side).setGlowing(is_glowing); }
    [[nodiscard]] bool getIsWaxed() const { return is_waxed_; }
    void setWaxed(bool waxed) { is_waxed_ = waxed; }
    [[nodiscard]] const ActorUniqueID &getLockedForEditingBy() const { return locked_for_editing_by_; }  // Endstone
    void setLockedForEditingBy(ActorUniqueID id) { locked_for_editing_by_ = id; }                        // Endstone
    void setMessageForServerScripingOnly(SignTextSide side, std::string message, std::string owner_id);

private:
    struct OpenSignRequest {
        bool is_waiting_for_change_clear;
        OpenSignPacket open_sign_packet;
        brstd::move_only_function<void(OpenSignPacket &)> send_packet;
    };

    Text &_getText(SignTextSide side) { return side == SignTextSide::Front ? *text_front_ : *text_back_; }
    [[nodiscard]] const Text &_getText(SignTextSide side) const
    {
        return side == SignTextSide::Front ? *text_front_ : *text_back_;
    }

    std::unique_ptr<Text> text_front_;
    std::unique_ptr<Text> text_back_;
    bool is_waxed_;
    ActorUniqueID locked_for_editing_by_;
    std::optional<OpenSignRequest> open_sign_request_;
    bool is_remote_profanity_filter_enabled_;
    bool is_local_profanity_filter_enabled_;
};
BEDROCK_STATIC_ASSERT_SIZE(SignBlockActor::Text, 408, 336);
BEDROCK_STATIC_ASSERT_SIZE(SignBlockActor, 424, 416);
