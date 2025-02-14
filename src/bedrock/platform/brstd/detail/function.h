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

namespace brstd::detail::function {

enum class DerivedType : int {
    MoveOnly = 0,
    Copyable = 1,
    Function = 2,
};

template <DerivedType, typename, bool>
class function_base_impl;

template <DerivedType Type, typename Return, bool Noexcept, typename... Xs>
class function_base_impl<Type, Return(Xs...), Noexcept> {
public:
    explicit operator bool() const
    {
        return _storage.vfptr != nullptr;
    }

    Return call(Xs... xs) const
    {
        return _storage.vfptr->invoke(_storage, static_cast<Xs &&>(xs)...);
    }

protected:
    void _construct_empty()
    {
        _storage = {nullptr};
    }

    template <typename T, typename CVRefT, typename... Ys>
    void _construct_target(T &&t)
    {
        _storage = {_create_vfptr<T, CVRefT>()};
        if constexpr (sizeof(T) <= storage::embedded_target_available_size) {
            ::new (static_cast<void *>(_storage.embedded_target_)) T(std::forward<T>(t));
        }
        else {
            _storage.heap_target() = new T(std::forward<T>(t));
        }
    }

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

    public:
        static constexpr size_t embedded_target_offset = 0;  // TODO: could be non-zero
        static constexpr size_t embedded_target_available_size = sizeof(embedded_target_) - embedded_target_offset;
        static constexpr bool is_heap_target = sizeof(T) > embedded_target_available_size;
        void *&heap_target()
        {
            return heap_target_;
        }
        void *const &heap_target() const
        {
            return heap_target_;
        }
    };
    static_assert(sizeof(storage) == 64);

    template <DerivedType>
    struct vtable_base;

    template <>
    struct vtable_base<DerivedType::MoveOnly> {
        void (*move_to)(storage &, storage &) = nullptr;  // +0
        void (*destroy)(storage &) = nullptr;             // +8
        Return (*invoke)(const storage &) = nullptr;      // +16
    };
    static_assert(sizeof(vtable_base<DerivedType::MoveOnly>) == 24);

    template <>
    struct vtable_base<DerivedType::Copyable> : vtable_base<DerivedType::MoveOnly> {
        void (*copy_to)(const storage &, storage &) = nullptr;  // +24
    };
    static_assert(sizeof(vtable_base<DerivedType::Copyable>) == 32);

    struct vtable : vtable_base<Type> {};

    template <typename ReturnA>
    struct invoker {
        template <typename... Ys>
        static ReturnA execute(Ys &&...);
    };

    template <typename T, typename CVRefT, bool HeapTarget>
    struct vtable_impl {
        static void *target(const storage &st)
        {
            // Return pointer to the stored T
            if constexpr (HeapTarget) {
                return st.heap_target_;
            }
            else {
                return static_cast<void *>(&st.embedded_target_);
            }
        }

        static void copy_to(const storage &from, storage &to)
        {
            // trivially, do a new T(*target(from)) in-place or on heap
            if constexpr (HeapTarget) {
                to.heap_target() = new T(*target(from));
            }
            else {
                ::new (static_cast<void *>(to.embedded_target_)) T(*target(from));
            }
        }

        static void move_to(storage &from, storage &to)
        {
            if constexpr (HeapTarget) {
                to.heap_target() = target(from);  // pointer move
                from.heap_target() = nullptr;
            }
            else {
                ::new (static_cast<void *>(to.embedded_target_)) T(std::move(*target(from)));
                // optionally destroy the old object in 'from'
                target(from)->~T();
            }
        }

        static void destroy(storage &st)
        {
            if constexpr (HeapTarget) {
                delete st.heap_target_;
                st.heap_target_ = nullptr;
            }
            else {
                target(st)->~T();
            }
        }

        static Return invoke(const storage &st, Xs... xs) noexcept(Noexcept)
        {
            return (*target(st))(static_cast<Xs &&>(xs)...);
        }
    };

    template <typename T, typename CVRefT>
    static const vtable *_create_vfptr()
    {
        static const vtable s_table = [] {
            vtable vt{};
            // Set up the function pointers:
            vt.move_to = &vtable_impl<T, CVRefT, storage::is_heap_target>::move_to;
            vt.destroy = &vtable_impl<T, CVRefT, storage::is_heap_target>::destroy;
            vt.invoke = &vtable_impl<T, CVRefT, storage::is_heap_target>::invoke;
            if constexpr (Type == DerivedType::Copyable) {
                vt.copy_to = &vtable_impl<T, CVRefT, storage::is_heap_target>::copy_to;
            }
            return vt;
        }();
        return &s_table;
    }

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

}  // namespace brstd::detail::function
