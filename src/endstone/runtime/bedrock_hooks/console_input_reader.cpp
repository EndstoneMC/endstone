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

#include "bedrock/server/console_input_reader.h"

#include "endstone/core/console/console.h"

using endstone::core::EndstoneConsole;

namespace {
struct ThreadProxyArgs {
#ifndef _WIN32
    void *thread_state;
#endif
    ConsoleInputReader *reader;
};
}  // namespace

void *ConsoleInputReader::run(void *arg)
{
    auto *self = static_cast<ThreadProxyArgs *>(arg)->reader;
    while (self->read_console_) {
        auto line = EndstoneConsole::getInstance().readLine("> ");
        if (line.has_value()) {
            if (!line->empty()) {
                self->console_input_.try_enqueue(line.value());
                if (line.value() == "stop") {
                    break;
                }
            }
        }
        else {
            // Ctrl + C, EOF
            self->console_input_.try_enqueue("stop");
            break;
        }
    }
    return nullptr;
}
