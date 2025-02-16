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
class copyable_function : public detail::function::function_invoke<detail::function::DerivedType::Copyable, Signature> {
    using base = detail::function::function_invoke<detail::function::DerivedType::Copyable, Signature>;

public:
    using base::base;
    using base::operator();
    copyable_function(copyable_function &&) noexcept = default;
    copyable_function &operator=(copyable_function &&) noexcept = default;
    copyable_function(copyable_function const &) = default;
    copyable_function &operator=(copyable_function const &) = default;
    template <class F>
        requires std::is_constructible_v<copyable_function, F>
    copyable_function &operator=(F &&f)
    {
        *this = copyable_function{std::forward<F>(f)};
        return *this;
    }
};
}  // namespace brstd
