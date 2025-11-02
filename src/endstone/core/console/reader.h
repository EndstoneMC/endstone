// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "bedrock/server/console_input_reader.h"
#include "endstone/core/console/console.h"

namespace endstone::core {
class EndstoneConsoleReader : public ConsoleInputReader {
public:
    EndstoneConsoleReader();
    [[nodiscard]] const EndstoneConsole &getConsole() const;
    [[nodiscard]] EndstoneConsole &getConsole();

private:
    EndstoneConsole console_;
};
}  // namespace endstone::core
