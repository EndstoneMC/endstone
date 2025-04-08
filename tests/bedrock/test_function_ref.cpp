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

#include <gtest/gtest.h>

#include "bedrock/platform/brstd/function_ref.h"

int free_function(int x)
{
    return x * 2;
}

struct Functor {
    int operator()(int x)
    {
        return x + 1;
    }
};

struct ConstFunctor {
    int operator()(int x) const
    {
        return x - 1;
    }
};

TEST(FunctionRefTests, FreeFunction)
{
    brstd::function_ref<int(int)> ref(free_function);
    EXPECT_EQ(ref(3), 6);
}

TEST(FunctionRefTests, Functor)
{
    Functor f;
    brstd::function_ref<int(int)> ref(f);
    EXPECT_EQ(ref(10), 11);
}

TEST(FunctionRefTests, ConstFunctor)
{
    ConstFunctor cf;
    brstd::function_ref<int(int)> ref(cf);
    EXPECT_EQ(ref(10), 9);
}

TEST(FunctionRefTests, Lambda)
{
    auto lambda = [](int x) {
        return x * x;
    };
    brstd::function_ref<int(int)> ref(lambda);
    EXPECT_EQ(ref(4), 16);
}

TEST(FunctionRefTest, CapturingLambda)
{
    int base = 10;
    auto lambda = [&base](int x) {
        return x + base;
    };
    brstd::function_ref<int(int)> ref(lambda);
    EXPECT_EQ(ref(5), 15);

    // Changing the captured value affects the result.
    base = 20;
    EXPECT_EQ(ref(5), 25);
}

TEST(FunctionRefTests, TemporaryLambda)
{
    brstd::function_ref<int(int)> ref([](int x) { return x + 100; });
    EXPECT_EQ(ref(1), 101);
}
