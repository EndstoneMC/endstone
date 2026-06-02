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

#include <memory>
#include <stdexcept>
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
}  // namespace

TEST(NotNullTest, ThrowsOnNull)
{
    EXPECT_THROW(NotNull<Base>(std::shared_ptr<Base>{}), std::invalid_argument);
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

TEST(NullableTest, Hashable)
{
    auto a = std::make_shared<Base>();
    std::unordered_set<Nullable<Base>> set;
    set.insert(Nullable<Base>(a));
    set.insert(Nullable<Base>(a));
    set.insert(Nullable<Base>{});
    EXPECT_EQ(set.size(), 2);
}
