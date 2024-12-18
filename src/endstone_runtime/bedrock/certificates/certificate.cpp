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

#include "bedrock/certificates/certificate.h"

Json::Value Certificate::getExtraData(const std::string &key, const Json::Value &default_value) const
{
    const auto extra_data = unverified_certificate_.raw_token_.getData().get("extraData", {});
    return extra_data.get(key, default_value);
}

bool Certificate::isValid() const
{
    return is_valid_;
}

bool Certificate::isSelfSigned() const
{
    return is_self_signed_;
}
