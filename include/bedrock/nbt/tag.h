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

#include "bedrock/common/util/data_io.h"
#include "bedrock/core/result.h"
#include "bedrock/core/utility/print_stream.h"

class Tag {
public:
    enum class Type : char {
        End = 0,
        Byte = 1,
        Short = 2,
        Int = 3,
        Int64 = 4,
        Float = 5,
        Double = 6,
        ByteArray = 7,
        String = 8,
        List = 9,
        Compound = 10,
        IntArray = 11,
        NumTagTypes = 12,
    };

    virtual ~Tag() = default;
    virtual void deleteChildren() {};
    virtual void write(IDataOutput &) const = 0;
    virtual Bedrock::Result<void> load(IDataInput &) = 0;
    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual Type getId() const = 0;
    [[nodiscard]] virtual bool equals(Tag const &other) const;
    virtual void print(std::string const &, PrintStream &stream) const;
    virtual void print(PrintStream &stream) const;
    [[nodiscard]] virtual std::unique_ptr<Tag> copy() const = 0;
    [[nodiscard]] virtual std::uint64_t hash() const = 0;

    static Bedrock::Result<std::unique_ptr<Tag>> newTag(Type);
    static std::string getTagName(Type type);
};
