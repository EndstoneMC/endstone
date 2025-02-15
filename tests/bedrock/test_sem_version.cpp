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

#include "bedrock/core/sem_ver/sem_version.h"
TEST(SemVersionTest, DefaultConstructor)
{
    SemVersion version;
    EXPECT_FALSE(version.isValid());
    EXPECT_FALSE(version.isAnyVersion());
    EXPECT_EQ(version.asString(), "0.0.0");
}

TEST(SemVersionTest, AnyVersionConstructor)
{
    SemVersion version(SemVersion::AnyVersionConstructor);
    EXPECT_TRUE(version.isValid());
    EXPECT_TRUE(version.isAnyVersion());
    EXPECT_EQ(version.asString(), "*");
}

TEST(SemVersionTest, ParameterizedConstructor)
{
    SemVersion version(1, 2, 3, "alpha", "build123");
    EXPECT_TRUE(version.isValid());
    EXPECT_FALSE(version.isAnyVersion());
    EXPECT_EQ(version.getMajor(), 1);
    EXPECT_EQ(version.getMinor(), 2);
    EXPECT_EQ(version.getPatch(), 3);
    EXPECT_EQ(version.getPreRelease(), "alpha");
    EXPECT_EQ(version.getBuildMeta(), "build123");
    EXPECT_EQ(version.asString(), "1.2.3-alpha+build123");
}

TEST(SemVersionTest, NoPreReleaseOrBuildMeta)
{
    SemVersion version(1, 2, 3);
    EXPECT_TRUE(version.isValid());
    EXPECT_FALSE(version.isAnyVersion());
    EXPECT_EQ(version.getMajor(), 1);
    EXPECT_EQ(version.getMinor(), 2);
    EXPECT_EQ(version.getPatch(), 3);
    EXPECT_EQ(version.getPreRelease(), nullptr);
    EXPECT_EQ(version.getBuildMeta(), nullptr);
    EXPECT_EQ(version.asString(), "1.2.3");
}

TEST(SemVersionTest, OnlyPreRelease)
{
    SemVersion version(1, 2, 3, "beta");
    EXPECT_EQ(version.asString(), "1.2.3-beta");
}

TEST(SemVersionTest, OnlyBuildMeta)
{
    SemVersion version(1, 2, 3, "", "build456");
    EXPECT_EQ(version.asString(), "1.2.3+build456");
}

TEST(SemVersionTest, ZeroVersionWithPreReleaseAndBuild)
{
    SemVersion version(0, 0, 0, "pre", "meta");
    EXPECT_EQ(version.asString(), "0.0.0-pre+meta");
}

TEST(SemVersionTest, EqualityOperator)
{
    SemVersion v1(1, 2, 3, "alpha", "build123");
    SemVersion v2(1, 2, 3, "alpha", "build123");
    SemVersion v3(1, 2, 3, "beta", "build123");
    SemVersion v4(1, 2, 3, "alpha");
    SemVersion v5(SemVersion::AnyVersionConstructor);
    SemVersion v6(SemVersion::AnyVersionConstructor);

    EXPECT_EQ(v1, v2);
    EXPECT_EQ(v1, v4);
    EXPECT_NE(v1, v3);
    EXPECT_EQ(v5, v6);
    EXPECT_NE(v1, v5);
    EXPECT_NE(v5, v1);
}

TEST(SemVersionTest, InequalityOperator)
{
    EXPECT_TRUE(SemVersion(1, 0, 0, "alpha") < SemVersion(1, 0, 0, "alpha.1"));
    EXPECT_TRUE(SemVersion(1, 0, 0, "alpha.1") < SemVersion(1, 0, 0, "alpha.beta"));
    EXPECT_TRUE(SemVersion(1, 0, 0, "alpha.beta") < SemVersion(1, 0, 0, "beta"));
    EXPECT_TRUE(SemVersion(1, 0, 0, "beta") < SemVersion(1, 0, 0, "beta.2"));
    EXPECT_TRUE(SemVersion(1, 0, 0, "beta.2") < SemVersion(1, 0, 0, "beta.11"));
    EXPECT_TRUE(SemVersion(1, 0, 0, "beta.11") < SemVersion(1, 0, 0, "rc.1"));
    EXPECT_TRUE(SemVersion(1, 0, 0, "rc.1") < SemVersion(1, 0, 0));
}
