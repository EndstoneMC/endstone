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

#include "bedrock/deps/json/value.h"

class WebToken {
public:
    explicit WebToken() = default;
    [[nodiscard]] const Json::Value &getHeader() const;
    [[nodiscard]] const Json::Value &getData() const;

private:
    std::string header_;       // +0
    Json::Value header_info_;  // +32
    std::string data_;         // +48
    Json::Value data_info_;    // +80
    std::string signature_;    // +96
};
