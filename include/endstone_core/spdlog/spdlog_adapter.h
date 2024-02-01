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

#include <spdlog/spdlog.h>

#include "endstone/logger.h"

class SpdLogAdapter : public Logger {
public:
    explicit SpdLogAdapter(std::shared_ptr<spdlog::logger> logger);
    void setLevel(Level level) override;
    [[nodiscard]] bool isEnabledFor(Level level) const override;
    [[nodiscard]] std::string_view getName() const override;
    void log(Level level, const std::string &message) const override;

private:
    std::shared_ptr<spdlog::logger> logger_;
};
