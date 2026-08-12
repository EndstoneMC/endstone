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

#include "endstone/core/block/sign.h"

#include <algorithm>
#include <cmath>

#include "endstone/check.h"

namespace endstone::core {

namespace {

int toByte(const float component)
{
    return std::clamp(static_cast<int>(std::lround(component * 255.0F)), 0, 255);
}

Color fromMinecraft(const mce::Color &color)
{
    return Color::fromRGBA(toByte(color.r), toByte(color.g), toByte(color.b), toByte(color.a));
}

mce::Color toMinecraft(const Color &color)
{
    mce::Color result;
    result.r = static_cast<float>(color.getRed()) / 255.0F;
    result.g = static_cast<float>(color.getGreen()) / 255.0F;
    result.b = static_cast<float>(color.getBlue()) / 255.0F;
    result.a = static_cast<float>(color.getAlpha()) / 255.0F;
    return result;
}

// Bedrock keeps the whole sign as one newline-separated message, so the lines are split out on the way in
std::vector<std::string> splitLines(const std::string &message)
{
    std::vector<std::string> lines;
    for (std::size_t start = 0; start <= message.size();) {
        const auto end = message.find('\n', start);
        if (end == std::string::npos) {
            lines.emplace_back(message.substr(start));
            break;
        }
        lines.emplace_back(message.substr(start, end - start));
        start = end + 1;
    }
    lines.resize(std::max(lines.size(), static_cast<std::size_t>(::SignBlockActor::NUM_LINES)));
    return lines;
}

std::string joinLines(const std::vector<std::string> &lines)
{
    std::string message;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i > 0) {
            message += '\n';
        }
        message += lines[i];
    }
    return message;
}

}  // namespace

EndstoneSignSide::EndstoneSignSide(EndstoneSign &sign, ::SignTextSide side) : sign_(sign), side_(side) {}

std::vector<std::string> EndstoneSignSide::getLines() const
{
    return splitLines(sign_.getSideData(side_).message);
}

std::string EndstoneSignSide::getLine(int index) const
{
    Preconditions::checkArgument(index >= 0 && index < ::SignBlockActor::NUM_LINES,
                                 "Line index must be between 0 and {}, got {}.", ::SignBlockActor::NUM_LINES - 1,
                                 index);
    return getLines()[index];
}

void EndstoneSignSide::setLine(int index, std::string line)
{
    Preconditions::checkArgument(index >= 0 && index < ::SignBlockActor::NUM_LINES,
                                 "Line index must be between 0 and {}, got {}.", ::SignBlockActor::NUM_LINES - 1,
                                 index);
    auto lines = getLines();
    lines[index] = std::move(line);
    sign_.getSideData(side_).message = joinLines(lines);
}

bool EndstoneSignSide::isGlowingText() const
{
    return sign_.getSideData(side_).glowing;
}

void EndstoneSignSide::setGlowingText(bool glowing)
{
    sign_.getSideData(side_).glowing = glowing;
}

Color EndstoneSignSide::getColor() const
{
    return fromMinecraft(sign_.getSideData(side_).color);
}

void EndstoneSignSide::setColor(Color color)
{
    sign_.getSideData(side_).color = toMinecraft(color);
}

EndstoneSign::EndstoneSign(const EndstoneBlock &block, ::SignBlockActor &sign)
    : EndstoneBlockStateBase<Sign>(block, sign),
      front_data_{sign.getMessage(::SignTextSide::Front), sign.getSignTextColor(::SignTextSide::Front),
                  sign.getIsGlowing(::SignTextSide::Front)},
      back_data_{sign.getMessage(::SignTextSide::Back), sign.getSignTextColor(::SignTextSide::Back),
                 sign.getIsGlowing(::SignTextSide::Back)},
      waxed_(sign.getIsWaxed()), front_(*this, ::SignTextSide::Front), back_(*this, ::SignTextSide::Back)
{
}

SignSide &EndstoneSign::getSide(Side side) const
{
    return side == Side::Front ? front_ : back_;
}

bool EndstoneSign::isWaxed() const
{
    return waxed_;
}

void EndstoneSign::setWaxed(bool waxed)
{
    waxed_ = waxed;
}

bool EndstoneSign::update()
{
    return update(false);
}

bool EndstoneSign::update(bool force)
{
    return update(force, true);
}

bool EndstoneSign::update(bool force, bool apply_physics)
{
    const auto block = getBlock();
    if (block->getType() != getType() && !force) {
        return false;
    }

    auto *block_entity = getBlockSource().getBlockEntity(block_pos_);
    if (block_entity == nullptr || (block_entity->getType() != BlockActorType::Sign &&
                                    block_entity->getType() != BlockActorType::HangingSign)) {
        return false;
    }

    block->setData(*getData(), apply_physics);
    block_entity = getBlockSource().getBlockEntity(block_pos_);
    if (block_entity == nullptr || (block_entity->getType() != BlockActorType::Sign &&
                                    block_entity->getType() != BlockActorType::HangingSign)) {
        return false;
    }

    auto &sign = static_cast<::SignBlockActor &>(*block_entity);
    sign.setMessageForServerScripingOnly(::SignTextSide::Front, front_data_.message, {});
    sign.setMessageForServerScripingOnly(::SignTextSide::Back, back_data_.message, {});
    sign.setSignTextColor(::SignTextSide::Front, front_data_.color);
    sign.setSignTextColor(::SignTextSide::Back, back_data_.color);
    sign.setIsGlowing(::SignTextSide::Front, front_data_.glowing);
    sign.setIsGlowing(::SignTextSide::Back, back_data_.glowing);
    sign.setWaxed(waxed_);
    return true;
}

EndstoneSign::SideData &EndstoneSign::getSideData(::SignTextSide side)
{
    return side == ::SignTextSide::Front ? front_data_ : back_data_;
}

const EndstoneSign::SideData &EndstoneSign::getSideData(::SignTextSide side) const
{
    return side == ::SignTextSide::Front ? front_data_ : back_data_;
}

}  // namespace endstone::core
