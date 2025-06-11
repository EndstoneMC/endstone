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

enum ValueType : char {  // NOLINTBEGIN
    nullValue = 0,       ///< 'null' value
    intValue = 1,        ///< signed integer value
    uintValue = 2,       ///< unsigned integer value
    realValue = 3,       ///< double value
    stringValue = 4,     ///< UTF-8 string value
    booleanValue = 5,    ///< bool value
    arrayValue = 6,      ///< array value (ordered list)
    objectValue = 7,     ///< object value (collection of name/value pairs).
};                       // NOLINTEND

class Value {
    static const Value &null;

    class CZString {
    public:
        CZString(const char *cstr);  // NOLINT(*-explicit-constructor)
        CZString(const CZString &other);
        ~CZString();
        bool operator<(const CZString &other) const;
        bool operator==(const CZString &other) const;
        [[nodiscard]] const char *c_str() const;

    private:
        const char *cstr_;
    };
    static_assert(sizeof(CZString) == 8);

public:
    using ObjectValues = std::map<CZString, Value>;
    using ArrayValues = std::vector<Value *>;
    using ArrayIndex = unsigned int;

    Value(ValueType type = nullValue);  // NOLINT(*-explicit-constructor)
    Value(const std::string &);
    Value(const Value &other);
    ~Value();
    Value &operator=(Value other);
    void swap(Value &other) noexcept;

    [[nodiscard]] ValueType type() const;
    [[nodiscard]] const char *asCString() const;
    [[nodiscard]] std::string asString() const;
    [[nodiscard]] int asInt() const;
    [[nodiscard]] std::int64_t asInt64() const;
    [[nodiscard]] std::uint64_t asUInt64() const;
    [[nodiscard]] double asDouble() const;
    [[nodiscard]] bool asBool() const;

    [[nodiscard]] std::vector<std::string> getMemberNames() const;

    /// Number of values in array or object
    [[nodiscard]] ArrayIndex size() const;

    /// \brief Append value to array at the end.
    ///
    /// Equivalent to jsonvalue[jsonvalue.size()] = value;
    Value &append(const Value &value);

    /// Resize the array to size elements.
    /// New elements are initialized to null.
    /// May only be called on nullValue or arrayValue.
    /// \pre type() is arrayValue or nullValue
    /// \post type() is arrayValue
    void resize(ArrayIndex size);

    /// Access an array element (zero based index).
    /// If the array contains less than index element, then null value are inserted
    Value &operator[](ArrayIndex index);
    /// Access an array element (zero based index).
    /// If the array contains less than index element, then null value are inserted
    Value &operator[](int index);
    /// Access an array element (zero based index)
    const Value &operator[](ArrayIndex index) const;
    /// Access an array element (zero based index)
    const Value &operator[](int index) const;
    /// Access an object value by name, create a null member if it does not exist.
    Value &operator[](const char *key);
    /// Access an object value by name, returns null if there is no member with that name.
    const Value &operator[](const char *key) const;
    /// Access an object value by name, create a null member if it does not exist.
    Value &operator[](const std::string &key);
    /// Access an object value by name, returns null if there is no member with that name.
    const Value &operator[](const std::string &key) const;
    /// Return the member named key if it exist, defaultValue otherwise.
    Value get(const char *key, const Value &default_value) const;
    /// Return the member named key if it exist, defaultValue otherwise.
    [[nodiscard]] Value get(const std::string &key, const Value &default_value) const;

private:
    Value &resolveReference(const char *key);

    union ValueHolder {
        std::int64_t int_;
        std::uint64_t uint_;
        double real_;
        bool bool_;
        CZString *string_;
        ArrayValues *array_;
        ObjectValues *map_;
    } value_;         // +0
    ValueType type_;  // +8
};
static_assert(sizeof(Value::ObjectValues::value_type) == 24);
static_assert(sizeof(Value) == 16);

}  // namespace Json
