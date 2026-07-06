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

namespace Bedrock {
class LogLevel {
public:
    enum class Type : std::uint8_t {
        Verbose = 1,
        Info = 2,
        Warning = 4,
        Error = 8,
    };

    struct InPublishT {};

    static const LogLevel Verbose;
    static const LogLevel Info;
    static const LogLevel Warning;
    static const LogLevel Error;
    static const InPublishT InPublish;

    constexpr LogLevel(Type type) : type_(type) {}
    [[nodiscard]] constexpr Type getType() const { return type_; }

    LogLevel operator|(const InPublishT &) const
    {
        LogLevel result = *this;
        result.log_in_publish_ = true;
        return result;
    }

private:
    Type type_;
    bool log_in_publish_{false};
};

inline const LogLevel LogLevel::Verbose{LogLevel::Type::Verbose};
inline const LogLevel LogLevel::Info{LogLevel::Type::Info};
inline const LogLevel LogLevel::Warning{LogLevel::Type::Warning};
inline const LogLevel LogLevel::Error{LogLevel::Type::Error};
inline const LogLevel::InPublishT LogLevel::InPublish{};
}  // namespace Bedrock
