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

#include <memory>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/platform/brstd/move_only_function.h"

class ActorInteraction {
public:
    using OnInteraction = brstd::move_only_function<void()>;

    [[nodiscard]] bool shouldCapture() const { return !no_capture_; }

    [[nodiscard]] const std::string &getInteractText() const { return interact_text_; }

    void suppressInteraction()  // Endstone
    {
        std::destroy_at(&interaction_);
        std::construct_at(&interaction_, [] {});
    }

private:
    std::string interact_text_;
    OnInteraction interaction_;
    bool no_capture_;
};

BEDROCK_STATIC_ASSERT_SIZE(ActorInteraction, 112, 112);
