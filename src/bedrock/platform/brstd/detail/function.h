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

#include <boost/utility/result_of.hpp>

namespace brstd {
namespace detail {
namespace function {
enum class DerivedType : int {
    MoveOnly = 0,
    Copyable = 1,
    Function = 2,
};

template <DerivedType, typename, bool>
class function_base_impl;

template <DerivedType Type, typename Return, bool Noexcept, typename... Xs>
class function_base_impl<Type, Return(Xs...), Noexcept> {
private:
    struct vtable;
    class storage {
    public:
        const vtable *vfptr;  // +0

    private:
        union {
            void *heap_target_;              // +0
            std::byte embedded_target_[56];  // +0
        };  // +8
    };
    static_assert(sizeof(storage) == 64);

    template <DerivedType>
    struct vtable_base {
        void (*move_to)(storage &, storage &);  // +0
        void (*destroy)(storage &);             // +8
        Return (*invoke)(const storage &);      // +16
    };

    struct vtable : vtable_base<Type> {};
    static_assert(sizeof(vtable) == 24);

protected:
    storage _storage;  // +0
};

template <DerivedType, typename Base>
class function_base;

template <typename Base>
class function_base<DerivedType::MoveOnly, Base> : public Base {};

template <typename Base>
class function_base<DerivedType::Copyable, Base> : public function_base<DerivedType::MoveOnly, Base> {};

template <DerivedType, typename Signature, bool OverrideCallOperatorModifiers>
class function_invoke_base;

template <typename Signature, bool OverrideCallOperatorModifiers>
class function_invoke_base<DerivedType::Copyable, Signature, OverrideCallOperatorModifiers>
    : public function_base<DerivedType::Copyable,
                           function_base_impl<DerivedType::Copyable, Signature, OverrideCallOperatorModifiers>> {};

template <DerivedType Type, typename Signature, bool OverrideCallOperatorModifiers>
class function_invoke : protected function_invoke_base<Type, Signature, OverrideCallOperatorModifiers> {};

}  // namespace function
}  // namespace detail
};  // namespace brstd
