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

#include <map>
#include <string>
#include <vector>

namespace Json {

enum ValueType {    // NOLINTBEGIN
    nullValue = 0,  ///< 'null' value
    intValue,       ///< signed integer value
    uintValue,      ///< unsigned integer value
    realValue,      ///< double value
    stringValue,    ///< UTF-8 string value
    booleanValue,   ///< bool value
    arrayValue,     ///< array value (ordered list)
    objectValue     ///< object value (collection of name/value pairs).
};  // NOLINTEND

class Value {
    static const Value &null;

    class CZString {
    public:
        CZString(const char *cstr);  // NOLINT(*-explicit-constructor)
        ~CZString();
        bool operator<(const CZString &other) const;
        bool operator==(const CZString &other) const;
        [[nodiscard]] const char *c_str() const;

    private:
        const char *cstr_;
    };
    static_assert(sizeof(Json::Value::CZString) == 8);

public:
    using ObjectValues = std::map<CZString, Value>;
    using ArrayValues = std::vector<Value*>;

    Value(ValueType type = nullValue);  // NOLINT(*-explicit-constructor)

    [[nodiscard]] ValueType type() const;
    [[nodiscard]] const char *asCString() const;
    [[nodiscard]] std::string asString() const;
    [[nodiscard]] int asInt() const;
    [[nodiscard]] std::int64_t asInt64() const;
    [[nodiscard]] std::uint64_t asUInt64() const;
    [[nodiscard]] double asDouble() const;
    [[nodiscard]] bool asBool() const;

    [[nodiscard]] std::vector<std::string> getMemberNames() const;
    [[nodiscard]] std::size_t size() const;

    /// Access an array element (zero based index)
    const Value &operator[](int index) const;
    /// Access an object value by name, returns null if there is no member with that name.
    const Value &operator[](const char *key) const;
    /// Return the member named key if it exist, defaultValue otherwise.
    Value get(const char *key, const Value &default_value) const;
    /// Return the member named key if it exist, defaultValue otherwise.
    [[nodiscard]] Value get(const std::string &key, const Value &default_value) const;

private:
    void initBasic(ValueType type, bool allocated = false);

    union ValueHolder {  // NOLINTBEGIN
        std::int64_t int_;
        std::uint64_t uint_;
        double real_;
        bool bool_;
        char **string_;
        ArrayValues *array_;
        ObjectValues *map_;
    } value_;  // NOLINTEND
    ValueType type_ : 8;
    int allocated_ : 1;  // Notes: if declared as bool, bitfield is useless.
};
static_assert(sizeof(Json::Value::ObjectValues::value_type) == 24);
static_assert(sizeof(Json::Value) == 16);

}  // namespace Json
