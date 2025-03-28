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

#include <fmt/format.h>

#include "endstone/util/error.h"

namespace endstone::core {

Error make_error(std::string_view message);

template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
Error make_error(const fmt::format_string<Args...> format, Args &&...args)
{
    return make_error(fmt::format(format, std::forward<Args>(args)...));
}

}  // namespace endstone::core
