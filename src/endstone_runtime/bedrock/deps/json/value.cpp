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

#include "bedrock/deps/json/value.h"

#include <cmath>
#include <limits>

#include <nlohmann/json.hpp>

namespace Json {

namespace {

const unsigned char kNull[sizeof(Value)] = {0};
const unsigned char &kNullRef = kNull[0];

char *duplicateStringValue(const char *value, unsigned int length = -1)
{
    if (length == -1) {
        length = static_cast<unsigned int>(strlen(value));
    }

    if (length >= std::numeric_limits<int>::max()) {
        length = std::numeric_limits<int>::max() - 1;
    }

    char *new_string = static_cast<char *>(malloc(length + 1));
    if (!new_string) {
        return nullptr;
    }

    memcpy(new_string, value, length);
    new_string[length] = 0;
    return new_string;
}

void releaseStringValue(char *value)
{
    free(value);
}

}  // namespace

const Value &Value::null = reinterpret_cast<const Value &>(kNullRef);

Value::CZString::CZString(const char *cstr) : cstr_(duplicateStringValue(cstr)) {}

Value::CZString::CZString(const CZString &other)
    : cstr_(other.cstr_ != nullptr ? duplicateStringValue(other.cstr_) : other.cstr_)
{
}

Value::CZString::~CZString()
{
    if (cstr_) {
        releaseStringValue(const_cast<char *>(cstr_));
    }
}

bool Value::CZString::operator<(const Value::CZString &other) const
{
    if (cstr_) {
        return strcmp(cstr_, other.cstr_) < 0;
    }
    return false;
}

bool Value::CZString::operator==(const Value::CZString &other) const
{
    if (cstr_) {
        return strcmp(cstr_, other.cstr_) == 0;
    }
    return true;
}

const char *Value::CZString::c_str() const
{
    return cstr_;
}

Json::Value::Value(Json::ValueType type)
{
    initBasic(type);
    switch (type) {
    case nullValue:
        break;
    case intValue:
    case uintValue:
        value_.int_ = 0;
        break;
    case realValue:
        value_.real_ = 0.0;
        break;
    case stringValue:
        value_.string_ = nullptr;
        break;
    case arrayValue:
        value_.array_ = new ArrayValues();
        break;
    case objectValue:
        value_.map_ = new ObjectValues();
        break;
    case booleanValue:
        value_.bool_ = false;
        break;
    default:
        return;
    }
}

Value::Value(const Value &other) : type_(other.type_), allocated_(false)
{
    switch (type_) {
    case nullValue:
    case intValue:
    case uintValue:
    case realValue:
    case booleanValue:
        value_ = other.value_;
        break;
    case stringValue:
        if (other.value_.string_) {
            value_.string_ = new CZString(*other.value_.string_);
            allocated_ = true;
        }
        else {
            value_.string_ = nullptr;
            allocated_ = false;
        }
        break;
    case arrayValue:
        value_.array_ = new ArrayValues(*other.value_.array_);
        break;
    case objectValue:
        value_.map_ = new ObjectValues(*other.value_.map_);
        break;
    default:
        break;
    }
}

Value::~Value()
{
    switch (type_) {
    case nullValue:
    case intValue:
    case uintValue:
    case realValue:
    case booleanValue:
        break;
    case stringValue:
        if (allocated_) {
            delete value_.string_;
        }
        break;
    case arrayValue:
        delete value_.array_;
        break;
    case objectValue:
        delete value_.map_;
        break;
    default:
        break;
    }
}

Value &Value::operator=(Value other)
{
    swap(other);
    return *this;
}

void Value::swap(Value &other) noexcept
{
    ValueType temp = type_;
    type_ = other.type_;
    other.type_ = temp;
    std::swap(value_, other.value_);
    int temp2 = allocated_;
    allocated_ = other.allocated_;
    other.allocated_ = temp2;
}

const Value &Value::operator[](int index) const
{
    if (type_ == nullValue) {
        return null;
    }

    if (index < 0 || index >= value_.array_->size()) {
        return null;
    }
    return *value_.array_->at(index);
}

const Value &Value::operator[](const char *key) const
{
    if (type_ != objectValue) {
        return null;
    }

    CZString actual_key(key);
    ObjectValues::const_iterator it = value_.map_->find(actual_key);
    if (it == value_.map_->end()) {
        return null;
    }
    return it->second;
}

Value Value::get(const char *key, const Value &default_value) const
{
    const Value *value = &((*this)[key]);
    return value == &null ? default_value : *value;
}

Value Value::get(const std::string &key, const Value &default_value) const
{
    return get(key.c_str(), default_value);
}

void Value::initBasic(ValueType type, bool allocated)
{
    type_ = type;
    allocated_ = allocated;
}

ValueType Value::type() const
{
    return type_;
}

const char *Value::asCString() const
{
    return value_.string_->c_str();
}

std::string Value::asString() const
{
    switch (type_) {
    case nullValue:
        return "";
    case stringValue:
        return value_.string_ ? std::string(value_.string_->c_str()) : "";
    case booleanValue:
        return value_.bool_ ? "true" : "false";
    case intValue:
        return std::to_string(value_.int_);
    case uintValue:
        return std::to_string(value_.uint_);
    case realValue:
        return std::to_string(value_.real_);
    default:
        return "";
    }
}

int Value::asInt() const
{
    switch (type_) {
    case intValue:
        return static_cast<int>(value_.int_);
    case uintValue:
        return static_cast<int>(value_.uint_);
    case realValue:
        return static_cast<int>(value_.real_);
    case nullValue:
        return 0;
    case booleanValue:
        return value_.bool_ ? 1 : 0;
    default:
        return 0;
    }
}

std::int64_t Value::asInt64() const
{
    switch (type_) {
    case intValue:
        return static_cast<std::int64_t>(value_.int_);
    case uintValue:
        return static_cast<std::int64_t>(value_.uint_);
    case realValue:
        return static_cast<std::int64_t>(value_.real_);
    case nullValue:
        return 0;
    case booleanValue:
        return value_.bool_ ? 1 : 0;
    default:
        return 0;
    }
}

std::uint64_t Value::asUInt64() const
{
    switch (type_) {
    case intValue:
        return static_cast<std::uint64_t>(value_.int_);
    case uintValue:
        return static_cast<std::uint64_t>(value_.uint_);
    case realValue:
        return static_cast<std::uint64_t>(value_.real_);
    case nullValue:
        return 0;
    case booleanValue:
        return value_.bool_ ? 1 : 0;
    default:
        return 0;
    }
}

double Value::asDouble() const
{
    switch (type_) {
    case intValue:
        return static_cast<double>(value_.int_);
    case uintValue:
        return static_cast<double>(value_.uint_);
    case realValue:
        return static_cast<double>(value_.real_);
    case nullValue:
        return 0;
    case booleanValue:
        return value_.bool_ ? 1.0 : 0.0;
    default:
        return 0.0;
    }
}

bool Value::asBool() const
{
    switch (type_) {
    case booleanValue:
        return value_.bool_;
    case nullValue:
        return false;
    case intValue:
        return value_.int_ != 0;
    case uintValue:
        return value_.uint_ != 0;
    case realValue:
        return static_cast<bool>(value_.real_);
    default:
        return false;
    }
}

std::vector<std::string> Value::getMemberNames() const
{
    std::vector<std::string> members;
    if (type_ == nullValue) {
        return members;
    }

    members.reserve(value_.map_->size());
    for (auto &[key, value] : *value_.map_) {
        members.emplace_back(key.c_str());
    }
    return members;
}

std::size_t Value::size() const
{
    switch (type_) {
    case arrayValue:
        return value_.array_->size();
    case objectValue:
        return value_.map_->size();
    default:
        return 0;
    }
}

void to_json(nlohmann::json &j, const Value &value)
{
    switch (value.type()) {
    case nullValue:
        break;
    case intValue: {
        j = value.asInt64();
        break;
    }
    case uintValue: {
        j = value.asUInt64();
        break;
    }
    case realValue: {
        j = value.asDouble();
        break;
    }
    case stringValue: {
        j = value.asString();
        break;
    }
    case booleanValue: {
        j = value.asBool();
        break;
    }
    case arrayValue: {
        for (int i = 0; i < value.size(); i++) {
            j.push_back(value[i]);
        }
        break;
    }
    case objectValue: {
        auto members = value.getMemberNames();
        for (auto &member : members) {
            j[member] = value[member.c_str()];
        }
        break;
    }
    }
}

}  // namespace Json
