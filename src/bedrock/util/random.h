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

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/forward.h"
#include "bedrock/util/thread_owner.h"
#include "bedrock/world/level/block_pos.h"

using RandomSeed = unsigned int;

class IRandom;
class IPositionalRandomFactory {
public:
    virtual ~IPositionalRandomFactory() = default;
    [[nodiscard]] virtual std::unique_ptr<IRandom> forBlockPos(const BlockPos &) const = 0;
    [[nodiscard]] virtual std::unique_ptr<IRandom> forString(const std::string &) const = 0;
};
static_assert(sizeof(IPositionalRandomFactory) == 8);

class IRandom {
public:
    virtual ~IRandom();
    virtual int nextInt() = 0;
    virtual int nextInt(int) = 0;
    virtual int64_t nextLong() = 0;
    virtual bool nextBoolean() = 0;
    virtual float nextFloat() = 0;
    virtual double nextDouble() = 0;
    virtual double nextGaussianDouble() = 0;
    virtual void consumeCount(unsigned int) = 0;
    virtual std::unique_ptr<IRandom> fork() = 0;
    virtual std::unique_ptr<IPositionalRandomFactory> forkPositional();
};

class Random : public IRandom {
    using result_type = uint32_t;

public:
    Random();
    Random(RandomSeed, bool);

    int nextInt() override;
    int nextInt(int) override;
    int64_t nextLong() override;
    bool nextBoolean() override;
    float nextFloat() override;
    double nextDouble() override;
    double nextGaussianDouble() override;
    void consumeCount(unsigned int) override;
    std::unique_ptr<IRandom> fork() override;

    void reset();
    void setSeed(RandomSeed);
    [[nodiscard]] RandomSeed getSeed() const;
    void clone(const Random &);
    static result_type max();
    static result_type min();
    result_type operator()();
    int operator()(int);
    int nextInt(int, int);
    int nextIntInclusive(int, int);
    uint32_t nextUnsignedInt();
    uint32_t nextUnsignedInt(unsigned int);
    unsigned char nextUnsignedChar();
    float nextGaussian();

    int nextGaussianInt(int);
    float nextGaussianFloat();

    void next2float16(float &, float &);
    void next4float8(float &, float &, float &, float &);

    glm::vec3 nextVec3();
    glm::vec3 nextVec3(float);
    glm::vec3 nextGaussianVec3();
    [[nodiscard]] bool isFixedToConstant() const;
    static Random &getThreadLocal();

private:
    Bedrock::Application::ThreadOwner<Core::Random> random_;  // +32
};
