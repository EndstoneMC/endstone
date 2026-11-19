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

#include <string>
#include <vector>

#include "endstone/command/soft_enum.h"

namespace endstone::core {

class EndstoneCommandMap;
class EndstoneSoftEnum : public SoftEnum {
public:
    EndstoneSoftEnum(EndstoneCommandMap &command_map, std::string name);
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::vector<std::string> getValues() const override;
    void setValues(std::vector<std::string> values) override;
    void addValues(std::vector<std::string> values) override;
    void removeValues(std::vector<std::string> values) override;

private:
    EndstoneCommandMap &command_map_;
    std::string name_;
};

}  // namespace endstone::core
