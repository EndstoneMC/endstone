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

#include "endstone/util/pointers.h"

#include <concepts>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>

#include <gtest/gtest.h>

using endstone::Nullable;
using endstone::NotNull;

namespace {
struct Base {
    virtual ~Base() = default;
    int value = 0;
};
struct Derived : Base {};

struct Tagged {
    virtual ~Tagged() = default;
    int tag = 7;
};

struct Shape : endstone::Object {
    [[nodiscard]] endstone::ClassInfo getClassInfo() const override { return typeid(Shape); }
    [[nodiscard]] bool isInstanceOf(endstone::ClassInfo target) const override { return target == typeid(Shape); }
};

// Tagged comes first so the Object subobject sits at a non-zero offset within Circle.
struct Circle : Tagged, Shape {
    [[nodiscard]] endstone::ClassInfo getClassInfo() const override { return typeid(Circle); }
    [[nodiscard]] bool isInstanceOf(endstone::ClassInfo target) const override
    {
        return target == typeid(Circle) || Shape::isInstanceOf(target);
    }
};

struct Square : Shape {
    [[nodiscard]] endstone::ClassInfo getClassInfo() const override { return typeid(Square); }
    [[nodiscard]] bool isInstanceOf(endstone::ClassInfo target) const override
    {
        return target == typeid(Square) || Shape::isInstanceOf(target);
    }
};

template <class Handle, class U>
concept CanNarrow = requires(const Handle &h) {
    { h.template is<U>() } -> std::same_as<bool>;
    h.template as<U>();
};
}  // namespace

TEST(NotNullTest, RejectsNullptrAtCompileTime)
{
    // NotNull states the contract without checking it, so nullptr is the only null it can catch.
    static_assert(!std::is_constructible_v<NotNull<Base>, std::nullptr_t>);
    static_assert(!std::is_assignable_v<NotNull<Base> &, std::nullptr_t>);
    static_assert(!std::is_default_constructible_v<NotNull<Base>>);
}

TEST(NotNullTest, DereferenceAndAccess)
{
    auto ptr = std::make_shared<Base>();
    ptr->value = 42;
    NotNull<Base> handle(ptr);
    EXPECT_EQ(handle->value, 42);
    EXPECT_EQ((*handle).value, 42);
    EXPECT_EQ(handle.get(), ptr);
}

TEST(NotNullTest, CovariantConstruction)
{
    auto derived = std::make_shared<Derived>();
    NotNull<Base> from_shared = derived;          // shared_ptr<Derived> -> NotNull<Base>
    NotNull<Derived> derived_handle(derived);
    NotNull<Base> from_notnull = derived_handle;  // NotNull<Derived> -> NotNull<Base>
    EXPECT_EQ(from_shared.get(), derived);
    EXPECT_EQ(from_notnull.get(), derived);
}

TEST(NotNullTest, ConstructionFromNullable)
{
    auto ptr = std::make_shared<Base>();
    const Nullable<Base> filled(ptr);
    const NotNull<Base> handle = filled;
    EXPECT_EQ(handle.get(), ptr);

    const Nullable<Base> empty;
    EXPECT_EQ(NotNull<Base>{empty}.get(), nullptr);  // unchecked, the caller owns the contract
}

TEST(NotNullTest, Cast)
{
    auto derived = std::make_shared<Derived>();
    const NotNull<Base> base(derived);
    EXPECT_EQ(base.cast<Derived>().get(), derived);
}

TEST(NotNullTest, IsAndAs)
{
    auto circle = std::make_shared<Circle>();
    const NotNull<Shape> shape(circle);

    EXPECT_TRUE(shape.is<Circle>());
    EXPECT_TRUE(shape.is<Shape>());
    EXPECT_FALSE(shape.is<Square>());

    const Nullable<Circle> narrowed = shape.as<Circle>();
    EXPECT_TRUE(static_cast<bool>(narrowed));
    EXPECT_EQ(narrowed.get(), circle);
    EXPECT_EQ(narrowed->tag, 7);

    EXPECT_FALSE(static_cast<bool>(shape.as<Square>()));
}

TEST(NotNullTest, AsSharesOwnership)
{
    auto circle = std::make_shared<Circle>();
    const NotNull<Shape> shape(circle);
    EXPECT_EQ(circle.use_count(), 2);
    {
        const Nullable<Circle> narrowed = shape.as<Circle>();
        EXPECT_EQ(circle.use_count(), 3);
    }
    EXPECT_EQ(circle.use_count(), 2);
}

TEST(NotNullTest, IsAndAsRequireObject)
{
    static_assert(CanNarrow<NotNull<Shape>, Circle>);
    static_assert(!CanNarrow<NotNull<Shape>, Base>);
    static_assert(!CanNarrow<NotNull<Base>, Shape>);
    static_assert(!CanNarrow<NotNull<Base>, Base>);
    static_assert(!CanNarrow<Nullable<Base>, Base>);
}

TEST(NotNullTest, ComparisonAndHash)
{
    auto a = std::make_shared<Base>();
    auto b = std::make_shared<Base>();
    NotNull<Base> ha(a);
    NotNull<Base> ha2(a);
    NotNull<Base> hb(b);
    EXPECT_EQ(ha, ha2);
    EXPECT_NE(ha, hb);

    std::unordered_set<NotNull<Base>> set;
    set.insert(ha);
    set.insert(ha2);  // same underlying pointer -> no new entry
    set.insert(hb);
    EXPECT_EQ(set.size(), 2);
}

TEST(NullableTest, NullAndValueSemantics)
{
    Nullable<Base> empty;
    EXPECT_FALSE(static_cast<bool>(empty));
    EXPECT_TRUE(empty == nullptr);
    EXPECT_THROW(empty.value(), std::runtime_error);

    auto fallback = std::make_shared<Base>();
    EXPECT_EQ(empty.value_or(fallback), fallback);

    auto ptr = std::make_shared<Base>();
    Nullable<Base> filled(ptr);
    EXPECT_TRUE(static_cast<bool>(filled));
    EXPECT_EQ(&filled.value(), ptr.get());
    EXPECT_EQ(filled.value_or(fallback), ptr);
}

TEST(NullableTest, ConvertsFromNotNullAndCovariant)
{
    auto derived = std::make_shared<Derived>();
    NotNull<Derived> not_null(derived);
    Nullable<Base> from_not_null = not_null;   // NotNull<Derived> -> Nullable<Base>
    Nullable<Base> from_shared = derived;      // shared_ptr<Derived> -> Nullable<Base>
    EXPECT_EQ(from_not_null.get(), derived);
    EXPECT_EQ(from_shared.get(), derived);
}

TEST(NullableTest, Cast)
{
    auto derived = std::make_shared<Derived>();
    const Nullable<Base> base(derived);
    EXPECT_EQ(base.cast<Derived>().get(), derived);

    const Nullable<Base> empty;
    EXPECT_FALSE(static_cast<bool>(empty.cast<Derived>()));
}

TEST(NullableTest, IsAndAs)
{
    auto circle = std::make_shared<Circle>();
    const Nullable<Shape> shape(circle);

    EXPECT_TRUE(shape.is<Circle>());
    EXPECT_FALSE(shape.is<Square>());
    EXPECT_EQ(shape.as<Circle>().get(), circle);
    EXPECT_FALSE(static_cast<bool>(shape.as<Square>()));

    const Nullable<Shape> empty;
    EXPECT_FALSE(empty.is<Circle>());
    EXPECT_FALSE(empty.is<Shape>());
    EXPECT_FALSE(static_cast<bool>(empty.as<Circle>()));
}

TEST(NullableTest, Hashable)
{
    auto a = std::make_shared<Base>();
    std::unordered_set<Nullable<Base>> set;
    set.insert(Nullable<Base>(a));
    set.insert(Nullable<Base>(a));
    set.insert(Nullable<Base>{});
    EXPECT_EQ(set.size(), 2);
}

TEST(PointersTest, CrossWrapperComparison)
{
    auto a = std::make_shared<Base>();
    auto b = std::make_shared<Base>();
    const NotNull<Base> not_null(a);
    const Nullable<Base> same(a);
    const Nullable<Base> other(b);
    const Nullable<Base> empty;

    EXPECT_TRUE(not_null == same);
    EXPECT_TRUE(same == not_null);
    EXPECT_FALSE(not_null == other);
    EXPECT_FALSE(other == not_null);
    EXPECT_FALSE(not_null == empty);
    EXPECT_FALSE(empty == not_null);
}

TEST(PointersTest, CrossWrapperComparisonCovariant)
{
    auto derived = std::make_shared<Derived>();
    const NotNull<Derived> not_null(derived);
    const Nullable<Base> as_base(derived);
    const Nullable<Base> other(std::make_shared<Base>());

    EXPECT_TRUE(not_null == as_base);
    EXPECT_TRUE(as_base == not_null);
    EXPECT_FALSE(not_null == other);
}
