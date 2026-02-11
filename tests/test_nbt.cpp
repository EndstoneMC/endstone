// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <gtest/gtest.h>

#include "endstone/nbt/tag.h"

using namespace endstone;

TEST(NbtValueTag, EqualityAndImplicitReadConversion)
{
    ByteTag b5{5};
    ByteTag b6{6};
    EXPECT_TRUE(b5 == b5);
    EXPECT_TRUE(b5 != b6);

    StringTag s1{"hi"};
    StringTag s2{"bye"};
    EXPECT_TRUE(s1 == "hi");
    EXPECT_TRUE("hi" == s1);
    EXPECT_TRUE(s1 != s2);
    const std::string &sv = s1;  // implicit conversion to std::string
    EXPECT_EQ(sv, "hi");
}

TEST(NbtValueTag, GetReturnsConstRefForValueWrappers)
{
    nbt::Tag t = CompoundTag{
        {"name", StringTag{"Beacon"}},
        {"level", IntTag{4}},
    };
    {
        auto &&ref = t["name"].get<StringTag>();
        static_assert(std::is_same_v<std::remove_cvref_t<decltype(ref)>, StringTag>,
                      "Should return a reference to StringTag");
        std::string s = ref;  // implicit conversion to std::string
        EXPECT_EQ(s, "Beacon");
    }
    {
        auto &&ref = t["level"].get<IntTag>();
        static_assert(std::is_same_v<std::remove_cvref_t<decltype(ref)>, IntTag>,
                      "Should return a reference to IntTag");
        int v = ref;  // implicit conversion to int
        EXPECT_EQ(v, 4);
    }
}

TEST(NbtArrayTag, BracedInitAndMutationAndEquality)
{
    nbt::Tag t = CompoundTag{
        {"ba", ByteArrayTag{1, 2, 3}},
        {"ia", IntArrayTag{1, 2, 3, 4}},
    };
    auto &ba = t["ba"].get<ByteArrayTag>();
    ASSERT_EQ(ba.size(), 3);
    EXPECT_EQ(ba[0], 1);
    EXPECT_EQ(ba[1], 2);
    EXPECT_EQ(ba[2], 3);

    ba.push_back(255);
    EXPECT_EQ(ba.size(), 4);
    EXPECT_EQ(ba[3], 255);
    ba[1] = 7;
    EXPECT_EQ(ba[1], 7);

    // Equality
    ByteArrayTag a{1, 2, 3, 255}, b{1, 2, 3, 255}, c{3, 2, 1};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);

    // IntArrayTag equality
    IntArrayTag i1{1, 2, 3, 4};
    IntArrayTag i2{1, 2, 3, 4};
    IntArrayTag i3{1, 2, 4, 3};
    EXPECT_TRUE(i1 == i2);
    EXPECT_FALSE(i1 == i3);
}

TEST(NbtListTag, Homogeneity)
{
    // Start as empty list
    nbt::Tag lst = ListTag{};
    EXPECT_EQ(lst.type(), nbt::Type::List);
    EXPECT_TRUE(lst.empty());

    // First non-End fixes type
    lst.emplace_back(DoubleTag{1.0});
    EXPECT_EQ(lst.get<ListTag>().type(), nbt::Type::Double);
    ASSERT_EQ(lst.size(), 1);
    EXPECT_DOUBLE_EQ(lst.at(0).get<DoubleTag>(), 1.0);

    // Pushing another Double is fine
    lst.emplace_back(DoubleTag{2.5});
    EXPECT_EQ(lst.size(), 2);
    EXPECT_DOUBLE_EQ(lst.at(1).get<DoubleTag>(), 2.5);

    // Inserting a mismatched kind should throw (heterogeneous list disallowed)
    EXPECT_THROW(lst.emplace_back(IntTag{1}), std::invalid_argument);
}

TEST(NbtCompoundTag, BasicOpsAndDeterminism)
{
    CompoundTag c;
    EXPECT_TRUE(c.empty());

    // operator[] inserts End if missing
    auto &slot = c["Damage"];
    EXPECT_EQ(slot.type(), nbt::Type::End);

    // explicit assignment replaces node (kind reassignment allowed)
    c["Damage"] = ShortTag{5};
    EXPECT_EQ(c.at("Damage").type(), nbt::Type::Short);
    EXPECT_EQ(c.at("Damage").get<ShortTag>(), static_cast<short>(5));

    c["Damage"] = IntTag{6};
    EXPECT_EQ(c.at("Damage").type(), nbt::Type::Int);
    EXPECT_EQ(c.at("Damage").get<IntTag>(), 6);

    // contains/count
    EXPECT_TRUE(c.contains("Damage"));
    EXPECT_FALSE(c.contains("Unbreakable"));

    // insert_or_assign
    auto [it, inserted] = c.insert_or_assign("Name", StringTag{"Sword"});
    EXPECT_TRUE(inserted || it->first == "Name");
    EXPECT_EQ(it->second.get<StringTag>(), "Sword");

    // merge (move non-conflicting), conflicting key should be kept from 'c'
    CompoundTag other{
        {"Owner", StringTag{"Alex"}},
        {"Damage", ShortTag{10}},
    };
    c.merge(other);
    EXPECT_TRUE(c.contains("Owner"));
    EXPECT_EQ(c.at("Owner").get<StringTag>(), "Alex");
    EXPECT_EQ(c.at("Damage").get<IntTag>(), 6);  // unchanged
}

TEST(NbtTag, SelfShapingAndGetConstnessAndEquality)
{
    nbt::Tag t;  // End
    EXPECT_EQ(t.type(), nbt::Type::End);

    // Self-shape to compound on key write
    t["name"] = StringTag{"Beacon"};
    EXPECT_EQ(t.type(), nbt::Type::Compound);

    // Self-shape to list on index write
    nbt::Tag arr;  // End
    arr.emplace_back(IntTag{5});
    EXPECT_EQ(arr.type(), nbt::Type::List);
    EXPECT_EQ(arr.get<ListTag>().size(), 1);
    EXPECT_EQ(arr.get<ListTag>().at(0).get<IntTag>(), 5);

    {
        auto &&ref = t["name"].get<StringTag>();
        std::string s = ref;
        EXPECT_EQ(s, "Beacon");
    }
    {
        t["bytes"] = ByteArrayTag{1, 2, 3};
        auto &ref = t["bytes"].get<ByteArrayTag>();  // mutable ref
        ref.push_back(9);
        EXPECT_EQ(ref.size(), 4);
    }

    // Deep equality (compound + list)
    nbt::Tag t2 = CompoundTag{
        {"name", StringTag{"Beacon"}},
        {"bytes", ByteArrayTag{1, 2, 3, 9}},
    };
    EXPECT_TRUE(t == t2);
}

TEST(NbtStatic, ExplicitTypingNoGuessing)
{
    // There should be NO constructor from primitive ints/doubles/strings for Tag.
    static_assert(!std::is_constructible_v<nbt::Tag, int>, "Tag must not be constructible from int (no guessing)");
    static_assert(!std::is_constructible_v<nbt::Tag, double>,
                  "Tag must not be constructible from double (no guessing)");
    static_assert(!std::is_constructible_v<nbt::Tag, const char *>,
                  "Tag must not be constructible from C-string (no guessing)");
    static_assert(!std::is_constructible_v<nbt::Tag, std::string>,
                  "Tag must not be constructible from std::string (no guessing)");
}

TEST(NbtStatic, NoImplicitEmptyCompoundBraceAssignment)
{
    // Programmatic `{}` must NOT be accepted as a value to mean empty compound.
    // We cannot assert a compile error inside a runtime test, but we can approximate:
    // - Tag should NOT be constructible from a naked empty initializer_list that could bind `{}`.
    static_assert(!std::is_constructible_v<nbt::Tag, std::initializer_list<int>>,
                  "Tag must not accept raw brace-init `{}` as a value");
}

TEST(NbtExamples, NestedCompound)
{
    nbt::Tag root = CompoundTag{{"display",
                                 CompoundTag{
                                     {"Name", StringTag{"Sword"}},
                                     {"Lore",
                                      ListTag{
                                          StringTag{"Sharp"},
                                          StringTag{"Ancient"},
                                      }},
                                 }},
                                {"attributes", CompoundTag{}},
                                {"entities", ListTag{}},
                                {"matrix", ListTag{
                                               ListTag{
                                                   IntTag{1},
                                                   IntTag{2},
                                                   IntTag{3},
                                               },
                                               ListTag{
                                                   IntTag{4},
                                                   IntTag{5},
                                                   IntTag{6},
                                               },
                                           }}};

    // Quick structure checks
    auto &display = root["display"].get<CompoundTag>();
    EXPECT_TRUE(display.contains("Name"));
    EXPECT_EQ(display.at("Name").get<StringTag>(), "Sword");

    auto &lore = display.at("Lore").get<ListTag>();
    ASSERT_EQ(lore.size(), 2);
    EXPECT_EQ(lore.at(0).get<StringTag>(), "Sharp");
    EXPECT_EQ(lore.at(1).get<StringTag>(), "Ancient");

    auto &matrix = root["matrix"].get<ListTag>();
    ASSERT_EQ(matrix.size(), 2);
    auto &row0 = matrix.at(0).get<ListTag>();
    auto &row1 = matrix.at(1).get<ListTag>();
    ASSERT_EQ(row0.size(), 3);
    ASSERT_EQ(row1.size(), 3);
    EXPECT_EQ(row0.at(0).get<IntTag>(), 1);
    EXPECT_EQ(row1.at(2).get<IntTag>(), 6);
}

TEST(NbtTag, GetIfReturnsNullptrOnMismatch)
{
    nbt::Tag t = CompoundTag{
        {"k", IntTag{1}},
    };
    // Mismatch
    EXPECT_EQ(t.get_if<StringTag>(), nullptr);
    // Match via at()
    const nbt::Tag &cv = t.at("k");
    EXPECT_NE(cv.get_if<IntTag>(), nullptr);
}

TEST(NbtEquality, DeepStructural)
{
    nbt::Tag a = CompoundTag{
        {"a", IntTag{1}},
        {"b", ByteArrayTag{1, 2, 3}},
        {"c",
         ListTag{
             StringTag{"x"},
             StringTag{"y"},
         }},
    };
    nbt::Tag b = CompoundTag{
        {"a", IntTag{1}},
        {"b", ByteArrayTag{1, 2, 3}},
        {"c",
         ListTag{
             StringTag{"x"},
             StringTag{"y"},
         }},
    };
    EXPECT_TRUE(a == b);
    // mutate b slightly
    b["b"].get<ByteArrayTag>()[1] = 9;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(NbtCompoundTag, SelfAssignment)
{
    CompoundTag a{{"a", IntTag{1}}};
    a["b"] = a;

    // a now has two keys
    EXPECT_EQ(a.size(), 2);
    EXPECT_TRUE(a.contains("a"));
    EXPECT_TRUE(a.contains("b"));
    EXPECT_EQ(a.at("a").get<IntTag>(), 1);

    // a["b"] is a CompoundTag (deep copy snapshot taken BEFORE insertion)
    EXPECT_EQ(a.at("b").type(), nbt::Type::Compound);
    auto &inner = a.at("b").get<CompoundTag>();
    EXPECT_EQ(inner.size(), 1);
    EXPECT_EQ(inner.at("a").get<IntTag>(), 1);
}
