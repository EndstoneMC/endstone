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

#include <string>
#include <vector>

#include "bedrock/world/level/block/actor/sign_block_actor.h"
#include "endstone/block/sign.h"
#include "endstone/core/block/block_state.h"

namespace endstone::core {

class EndstoneSign;

class EndstoneSignSide : public SignSide {
public:
    EndstoneSignSide(const EndstoneSign &sign, ::SignTextSide side);

    [[nodiscard]] std::vector<std::string> getLines() const override;
    [[nodiscard]] std::string getLine(int index) const override;
    void setLine(int index, std::string line) override;
    [[nodiscard]] bool isGlowingText() const override;
    void setGlowingText(bool glowing) override;
    [[nodiscard]] Color getColor() const override;
    void setColor(Color color) override;

private:
    [[nodiscard]] ::SignBlockActor &getSign() const;

    const EndstoneSign &sign_;
    ::SignTextSide side_;
};

class EndstoneSign : public EndstoneBlockStateBase<Sign> {
public:
    EndstoneSign(const EndstoneBlock &block, ::SignBlockActor &sign);

    [[nodiscard]] SignSide &getSide(Side side) const override;
    [[nodiscard]] bool isWaxed() const override;
    void setWaxed(bool waxed) override;

private:
    friend class EndstoneSignSide;

    [[nodiscard]] ::SignBlockActor &getSign() const { return getBlockActor<::SignBlockActor>(); }
    mutable EndstoneSignSide front_;
    mutable EndstoneSignSide back_;
};

}  // namespace endstone::core
