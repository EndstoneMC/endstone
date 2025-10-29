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

#include <unordered_map>
#include <variant>

#include "bedrock/core/utility/pub_sub/publisher.h"

using PackSettingValue = std::variant<float, bool, std::string>;
struct PackSettingValueAndDefault {
    PackSettingValue value;
    PackSettingValue default_value;
};
using PackSettingsNameValueMap = std::unordered_map<std::string, PackSettingValueAndDefault>;

class PackSettings {
public:
private:
    const mce::UUID pack_id_;
    std::weak_ptr<Bedrock::PubSub::Publisher<void(const mce::UUID &, const std::string &,
                                                  const std::variant<float, bool, std::string> &),
                                             Bedrock::PubSub::ThreadModel::MultiThreaded>>
        on_change_publisher_;
    PackSettingsNameValueMap name_value_map_;
    uint64_t last_used_;
};
