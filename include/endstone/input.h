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

namespace endstone {

/**
 * Represents a movement input applied to a player.
 */
class Input {
public:
    constexpr Input() = default;

    constexpr Input(bool forward, bool backward, bool left, bool right, bool jump, bool sneak, bool sprint)
        : forward_(forward), backward_(backward), left_(left), right_(right), jump_(jump), sneak_(sneak),
          sprint_(sprint)
    {
    }

    Input(const Input &) = default;
    Input(Input &&) noexcept = default;
    Input &operator=(const Input &) = default;
    Input &operator=(Input &&) noexcept = default;

    /**
     * Gets whether a forward input is applied.
     *
     * @return true if a forward input is applied
     */
    [[nodiscard]] constexpr bool isForward() const { return forward_; }

    /**
     * Gets whether a backward input is applied.
     *
     * @return true if a backward input is applied
     */
    [[nodiscard]] constexpr bool isBackward() const { return backward_; }

    /**
     * Gets whether a left input is applied.
     *
     * @return true if a left input is applied
     */
    [[nodiscard]] constexpr bool isLeft() const { return left_; }

    /**
     * Gets whether a right input is applied.
     *
     * @return true if a right input is applied
     */
    [[nodiscard]] constexpr bool isRight() const { return right_; }

    /**
     * Gets whether a jump input is applied.
     *
     * @return true if a jump input is applied
     */
    [[nodiscard]] constexpr bool isJump() const { return jump_; }

    /**
     * Gets whether a sneak input is applied.
     *
     * @return true if a sneak input is applied
     */
    [[nodiscard]] constexpr bool isSneak() const { return sneak_; }

    /**
     * Gets whether a sprint input is applied.
     *
     * @return true if a sprint input is applied
     */
    [[nodiscard]] constexpr bool isSprint() const { return sprint_; }

    constexpr bool operator==(const Input &) const = default;

private:
    bool forward_ = false;
    bool backward_ = false;
    bool left_ = false;
    bool right_ = false;
    bool jump_ = false;
    bool sneak_ = false;
    bool sprint_ = false;
};

}  // namespace endstone
