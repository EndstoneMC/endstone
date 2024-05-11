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

#include "bedrock/io/data_input.h"
#include "bedrock/io/data_output.h"
#include "bedrock/io/print_stream.h"
#include "bedrock/result.h"

class Tag {
public:
    enum  Type : char {
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

    Tag() = delete;
    Tag(const Tag &other) = default;
    Tag &operator=(const Tag &other) = default;
    Tag(Tag &&other) = default;
    Tag &operator=(Tag &&other) = default;

    virtual ~Tag() = default;
    virtual void deleteChildren()
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    virtual void write(IDataOutput &) const
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    virtual Bedrock::Result<void> load(IDataInput &)
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    [[nodiscard]] virtual std::string toString() const
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    [[nodiscard]] virtual Type getId() const
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    [[nodiscard]] virtual bool equals(Tag const &) const
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    virtual void print(std::string const &, PrintStream &) const
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    virtual void print(PrintStream &) const
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    [[nodiscard]] virtual std::unique_ptr<Tag> copy() const
    {
        throw std::runtime_error("Pure virtual function called!");
    };
    [[nodiscard]] virtual std::uint64_t hash() const
    {
        throw std::runtime_error("Pure virtual function called!");
    };

    inline static std::string getTagName(Type type)
    {
        switch (type) {
        case Type::End:
            return "TAG_End";
        case Type::Byte:
            return "TAG_Byte";
        case Type::Short:
            return "TAG_Short";
        case Type::Int:
            return "TAG_Int";
        case Type::Int64:
            return "TAG_Long";
        case Type::Float:
            return "TAG_Float";
        case Type::Double:
            return "TAG_Double";
        case Type::ByteArray:
            return "TAG_Byte_Array";
        case Type::String:
            return "TAG_String";
        case Type::List:
            return "TAG_List";
        case Type::Compound:
            return "TAG_Compound";
        case Type::IntArray:
            return "TAG_Int_Array";
        case Type::NumTagTypes:
        default:
            return "UNKNOWN";
        }
    }
};
