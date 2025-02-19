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

#include <functional>

// Reference: https://github.com/LiteLDev/LeviLamina/blob/fbb3c97/src/mc/platform/brstd/detail/function.h

namespace brstd::detail::function {

constexpr size_t embedded_target_size = (sizeof(void *) * 8) - sizeof(void *);

enum class DerivedType : int {
    MoveOnly = 0,
    Copyable = 1,
    Function = 2,
};

template <DerivedType Type, typename Return, bool Noexcept, typename... Xs>
class function_base_impl {
public:
    explicit operator bool() const
    {
        return storage_.vfptr != nullptr;
    }

private:
    struct vtable;

protected:
    template <typename T, typename CVRefT, typename... Ys>
    void _construct_target(Ys &&...args)
    {
        storage_.vfptr = _create_vfptr<T, CVRefT>();
        if constexpr (storage::template is_heap_target<T>) {
            auto ptr = std::make_unique<T>(std::forward<Ys>(args)...);
            storage_.heap_target = ptr.release();
        }
        else {
            ::new (&storage_.embedded_target + storage::template embedded_target_offset<T>)
                T(std::forward<Ys>(args)...);
        }
    }

private:
    // ====== storage ====== //
    class storage {
    public:
        const vtable *vfptr;  // +0

    private:
        template <DerivedType, typename, bool, typename...>
        friend class function_base_impl;

        union {
            void *heap_target;
            std::byte embedded_target[56];
        };  // +8

    public:
        template <class T>
        static constexpr size_t embedded_target_offset =
            alignof(T) <= sizeof(vtable *) ? 0 : (alignof(T) - sizeof(vtable *));

        template <class T>
        static constexpr size_t embedded_target_available_size = embedded_target_size - embedded_target_offset<T>;

        template <class T>
        static constexpr bool is_heap_target =
            sizeof(T) > embedded_target_available_size<T> || !std::is_nothrow_move_constructible_v<T>;
    };

    // ====== virtual table base ====== //
    template <DerivedType>
    struct vtable_base;

    template <>
    struct vtable_base<DerivedType::MoveOnly> {
        void (*move_to)(storage &, storage &);
        void (*destroy)(storage &);
        Return (*invoke)(const storage &);
    };

    template <>
    struct vtable_base<DerivedType::Copyable> : vtable_base<DerivedType::MoveOnly> {
        void (*copy_to)(const storage &, storage &);
    };

    // ====== virtual table ====== //
    struct vtable : vtable_base<Type> {};

    // ====== virtual table implementation ====== //
    template <typename T, typename CVRefT, bool HeapTarget>
    struct vtable_impl {
        static void *target(const storage &self)
        {
            if constexpr (HeapTarget) {
                return self.heap_target;
            }
            else {
                return reinterpret_cast<void *>(&const_cast<storage &>(self).embedded_target +
                                                storage::template embedded_target_offset<T>);
            }
        }

        static void copy_to(const storage &from, storage &to)
        {
            if constexpr (HeapTarget) {
                to.heap_target = ::new T(*target(from));
            }
            else {
                ::new (target(to)) T(*static_cast<T *>(target(from)));
            }
        }

        static void move_to(storage &from, storage &to)
        {
            if constexpr (HeapTarget) {
                to.heap_target = target(from);
                from.heap_target = nullptr;
            }
            else {
                ::new (target(to)) T(std::move(*static_cast<T *>(target(from))));
            }
        }

        static void destroy(storage &self)
        {
            if constexpr (HeapTarget) {
                delete target(self);
                target(self) = nullptr;
            }
            else {
                static_cast<T *>(target(self))->~T();
            }
        }

        static Return invoke(const storage &storage, Xs &&...args) noexcept(Noexcept)
        {
            if constexpr (std::is_void_v<Return>) {
                std::invoke(*static_cast<T *>(target(storage)), std::forward<Xs>(args)...);
                return;
            }
            else {
                return std::invoke(*static_cast<T *>(target(storage)), std::forward<Xs>(args)...);
            }
        }
    };

    template <typename T, typename CVRefT>
    static const vtable *_create_vfptr()
    {
        static const vtable vtable = [] {
            function_base_impl::vtable vt{};
            vt.move_to = &vtable_impl<T, CVRefT, storage ::template is_heap_target<T>>::move_to;
            vt.destroy = &vtable_impl<T, CVRefT, storage ::template is_heap_target<T>>::destroy;
            vt.invoke = &vtable_impl<T, CVRefT, storage ::template is_heap_target<T>>::invoke;
            if constexpr (Type == DerivedType::Copyable) {
                vt.copy_to = &vtable_impl<T, CVRefT, storage ::template is_heap_target<T>>::copy_to;
            }
            return vt;
        }();
        return &vtable;
    }

protected:
    storage storage_;
};

template <DerivedType, typename Base>
class function_base;

template <typename Base>
class function_base<DerivedType::MoveOnly, Base> : public Base {
public:
    ~function_base()
    {
        if (auto *vfptr = Base::storage_.vfptr) {
            vfptr->destroy(Base::storage_);
        }
    }
};

template <typename Base>
class function_base<DerivedType::Copyable, Base> : public function_base<DerivedType::MoveOnly, Base> {};

template <DerivedType, typename Signature, bool OverrideCallOperatorModifiers>
class function_invoke_base;

template <typename Return, typename... Xs>
class function_invoke_base<DerivedType::Copyable, Return(Xs...) const, false>
    : public function_base<DerivedType::Copyable, function_base_impl<DerivedType::Copyable, Return, false>> {};

template <DerivedType Type, typename Signature, bool OverrideCallOperatorModifiers>
class function_invoke : protected function_invoke_base<Type, Signature, OverrideCallOperatorModifiers> {
protected:
    template <template <typename> class Derived, typename F>
    void construct_from_function(F &&f)
    {
        using T = std::decay_t<F>;
        using CVRefT = std::add_lvalue_reference_t<F>;
        this->template _construct_target<T, CVRefT>(std::forward<F>(f));
    }
};

}  // namespace brstd::detail::function
