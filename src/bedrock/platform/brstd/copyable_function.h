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

#include "bedrock/platform/brstd/detail/function.h"

// Reference: https://github.com/LiteLDev/LeviLamina/blob/fbb3c97/src/mc/platform/brstd/copyable_function.h

namespace brstd {

template <class Signature>
class copyable_function
    : public detail::function::function_invoke<detail::function::DerivedType::Copyable, Signature, false> {
public:
    template <typename F>
    copyable_function(F &&f)
        requires(!std::is_same_v<copyable_function, std::remove_cvref_t<F>>)
    {
        this->template construct_from_function<brstd::copyable_function, F>(std::forward<F>(f));
    }
};

}  // namespace brstd
