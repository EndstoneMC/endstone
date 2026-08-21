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

EndstoneSignSide::EndstoneSignSide(const EndstoneSign &sign, ::SignTextSide side) : sign_(sign), side_(side) {}

::SignBlockActor &EndstoneSignSide::getSign() const
{
    return sign_.getSign();
}

std::vector<std::string> EndstoneSignSide::getLines() const
{
    return splitLines(getSign().getMessage(side_));
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
    getSign().setMessageForServerScripingOnly(side_, joinLines(lines), {});
}

bool EndstoneSignSide::isGlowingText() const
{
    return getSign().getIsGlowing(side_);
}

void EndstoneSignSide::setGlowingText(bool glowing)
{
    getSign().setIsGlowing(side_, glowing);
    getSign().setChanged();
}

Color EndstoneSignSide::getColor() const
{
    return fromMinecraft(getSign().getSignTextColor(side_));
}

void EndstoneSignSide::setColor(Color color)
{
    getSign().setSignTextColor(side_, toMinecraft(color));
    getSign().setChanged();
}

EndstoneSign::EndstoneSign(const EndstoneBlock &block, ::SignBlockActor &sign)
    : EndstoneBlockStateBase<Sign>(block, sign), front_(*this, ::SignTextSide::Front),
      back_(*this, ::SignTextSide::Back)
{
}

SignSide &EndstoneSign::getSide(Side side) const
{
    return side == Side::Front ? front_ : back_;
}

bool EndstoneSign::isWaxed() const
{
    return getSign().getIsWaxed();
}

void EndstoneSign::setWaxed(bool waxed)
{
    getSign().setWaxed(waxed);
    getSign().setChanged();
}

}  // namespace endstone::core
