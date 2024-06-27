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

#ifndef ENDSTONE_DISABLE_DEVTOOLS

#include <nlohmann/json.hpp>

#include "endstone/detail/server.h"

namespace endstone::detail {
class DevTools {
public:
    static void render();
    static void show();
    static void hide();

private:
    static void showAboutWindow(bool *open);
    static void showBlockWindow(bool *open, EndstoneServer *server, nlohmann::json_abi_v3_11_3::basic_json<> &json);
    // static void showCommandWindow(bool *open, EndstoneServer *server);
    // static void showItemWindow(bool *open, EndstoneServer *server);
};
}  // namespace endstone::detail

#endif
