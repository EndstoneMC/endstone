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

namespace brstd {

template <typename Signature>
class copyable_function
    : public detail::function::function_invoke<detail::function::DerivedType::Copyable, Signature, false> {
public:
    copyable_function() noexcept
    {
        this->_construct_empty();
    }
    copyable_function(nullptr_t)
    {
        this->_construct_empty();
    }
    template <typename F, typename = std::enable_if_t<std::is_invocable_v<F>>>
    copyable_function(F &&f)
    {
        this->template _construct_target<F, F>(std::forward<F>(f));
    }

    copyable_function(copyable_function &&) = default;
    copyable_function(const copyable_function &) = default;
    copyable_function &operator=(copyable_function &&) = default;
    copyable_function &operator=(const copyable_function &) = default;
    copyable_function &operator=(nullptr_t)
    {
        this->_construct_empty();
        return *this;
    }
};

};  // namespace brstd
