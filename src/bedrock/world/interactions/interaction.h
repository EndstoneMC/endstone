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

#include <string_view>

#include "bedrock/core/string/static_optimized_string.h"
#include "bedrock/platform/brstd/move_only_function.h"

class Interaction {
public:
    using ExecutionCallback = brstd::move_only_function<void()>;  // Endstone: BDS spells it `void() &&`

    Interaction() = default;  // Endstone: BDS spells this Interaction::Failure()
    [[nodiscard]] bool isSuccessful() const { return !interact_text_.view().empty(); }
    [[nodiscard]] bool shouldSwing() const { return swing_; }
    [[nodiscard]] std::string_view getInteractText() const { return interact_text_.view(); }

private:
    Bedrock::StaticOptimizedString interact_text_;
    ExecutionCallback callback_{nullptr};
    bool swing_ : 1 {false};
};
