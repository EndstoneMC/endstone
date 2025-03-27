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
#include <type_traits>

namespace brstd {
namespace detail {
union function_ref_storage {
    void *obj;
    const void *const_obj;
    void (*func)();
    explicit function_ref_storage();
    template <class T>
        requires std::is_object_v<T>
    constexpr explicit function_ref_storage(T *obj) noexcept : obj(obj)
    {
    }

    template <class T>
        requires std::is_object_v<T>
    constexpr explicit function_ref_storage(T const *obj) noexcept : const_obj(obj)
    {
    }

    template <class T>
        requires std::is_function_v<T>
    constexpr explicit function_ref_storage(T *f) noexcept : func(reinterpret_cast<decltype(func)>(f))
    {
    }
};
static_assert(sizeof(function_ref_storage) == 8);
}  // namespace detail

using function_ref_storage = detail::function_ref_storage;

template <typename Signature, typename = Signature>
class function_ref;

template <typename Return, typename... Args>
class function_ref<Return(Args...), Return(Args...)> {
    static constexpr bool is_noexcept = false;
    static constexpr bool is_invocable_using = std::is_invocable_v<Return(Args...), Args...>;

public:
    template <typename F, typename T = std::remove_reference_t<F>>
    constexpr function_ref(F &&f) noexcept : storage_(std::addressof(f))
    {
        func_ = [](function_ref_storage s, Args &&...args) noexcept(is_noexcept) -> Return {
            if constexpr (std::is_const_v<T>) {
                return (*static_cast<T const *>(s.const_obj))(std::forward<Args>(args)...);
            }
            else if constexpr (std::is_object_v<T>) {
                return (*static_cast<T *>(s.obj))(std::forward<Args>(args)...);
            }
            else {
                static_assert(std::is_function_v<T>, "Bad function type");
                return (*reinterpret_cast<T *>(s.func))(std::forward<Args>(args)...);
            }
        };
    }

    Return operator()(Args... args) const
    {
        return func_(storage_, std::forward<Args>(args)...);
    }

private:
    using function_t = Return(function_ref_storage, std::add_rvalue_reference_t<Args>...);
    function_t *func_;              // +0
    function_ref_storage storage_;  // +8
};

}  // namespace brstd
