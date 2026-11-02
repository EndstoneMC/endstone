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

#include <cstdint>

#include <glm/glm.hpp>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/i_random.h"
#include "bedrock/platform/uuid.h"

using RandomSeed = unsigned int;

namespace Core {
class Random : public IRandom {
public:
    using result_type = uint32_t;

    static Random &getThreadLocal();
    Random(RandomSeed seed, bool only_used_deterministically);
    static Random createSeedable(RandomSeed, bool);
    static Random createSeedable();
    Random(Random &&other);
    void reset();
    void setSeed(RandomSeed seed);
    [[nodiscard]] Random clone() const;
    void setSeedUnitTest(RandomSeed);
    [[nodiscard]] RandomSeed getSeed() const;
    bool nextBoolean() override;
    float nextFloat() override;
    float nextBellCurveFloat(float min, float max);
    float nextBellCurveFloat(float);
    double nextDouble() override;
    int nextInt() override;
    int64_t nextLong() override;
    int nextInt(int n) override;
    void consumeCount(unsigned int count) override;
    uint32_t nextUnsignedInt();
    uint32_t nextUnsignedInt(unsigned int n);
    uint64_t nextUnsignedInt64();
    unsigned char nextUnsignedChar();
    float nextGaussian();
    double nextGaussianDouble() override;
    int nextGaussianInt(int n);
    float nextGaussianFloat();
    void next2float16(float &, float &);
    void next4float8(float &, float &, float &, float &);
    glm::vec3 nextVec3();
    glm::vec3 nextVec3(float f);
    glm::vec3 nextGaussianVec3();
    static result_type max();
    static result_type min();
    result_type operator()();
    int operator()(int n);
    static mce::UUID generateUUID();
    static uint64_t generateUUID64Bit();
    [[nodiscard]] bool isFixedToConstant() const;

private:
    Random();
    Random(const Random &);
    Random &operator=(const Random &);
    RandomSeed seed_;
    uint32_t mt_[624];
    int mti_;
    int inited_idx_;
    float next_next_gaussian_;
    double next_next_gaussian_double_;
    result_type fake_uniform_random_int_;
    bool have_next_next_gaussian_;
    bool have_next_next_gaussian_double_;
    bool test_only_used_deterministically_;
    bool allow_seed_changes_;
    [[nodiscard]] bool _canChangeSeed() const;
    void _setSeed(RandomSeed seed);
    void _initGenRand(uint32_t s);
    void _initGenRandFast(uint32_t s);
    void _initByArray(uint32_t *, int);
    uint32_t _genRandInt32(bool allow_unit_test_deterministic_override);
    uint64_t _nextUnsignedInt64(bool allow_unit_test_deterministic_override);
    int _genRandInt31();
    double _genRandReal1();
    double _genRandReal2();
    double _genRandReal3();
    double _genRandRes53();
    void _rrDiff(float &);
    void _rrDiff(float &, float &);
    void _rrDiff(float &, float &, float &);
    result_type _genUniformRandomInt();
    static RandomSeed _generateCryptoSeed();
};
}  // namespace Core
BEDROCK_STATIC_ASSERT_SIZE(Core::Random, 2536, 2536);
