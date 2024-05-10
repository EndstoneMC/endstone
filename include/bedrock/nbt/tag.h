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

#include "bedrock/forward.h"
#include "bedrock/io/print_stream.h"

class Tag {
public:
    enum class Type {
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

    virtual ~Tag() = 0;
    virtual void deleteChildren() = 0;
    virtual void write(IDataOutput &) const = 0;
    virtual Bedrock::Result<void, std::error_code> load(IDataInput &) = 0;
    virtual std::string toString() const = 0;
    virtual Type getId() const = 0;
    virtual bool equals(Tag const &) const = 0;
    virtual void print(std::string const &, PrintStream &) const = 0;
    virtual void print(PrintStream &) const = 0;
    virtual std::unique_ptr<Tag> copy() const = 0;
    virtual std::uint64_t hash() const = 0;
};
