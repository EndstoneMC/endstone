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

#include <gtest/gtest.h>

#include "endstone/brigadier/exceptions.h"

namespace endstone::brigadier::support {

template <typename F>
void expectSyntaxError(F &&function, const CommandExceptionType &type, int cursor)
{
    try {
        function();
    }
    catch (const CommandSyntaxException &ex) {
        EXPECT_EQ(&ex.getType(), &type);
        EXPECT_EQ(ex.getCursor(), cursor);
        return;
    }
    ADD_FAILURE() << "expected a CommandSyntaxException";
}

}  // namespace endstone::brigadier::support
