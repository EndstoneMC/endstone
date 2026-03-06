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

#include <variant>

#include "endstone/debug/shape/arrow.h"
#include "endstone/debug/shape/box.h"
#include "endstone/debug/shape/circle.h"
#include "endstone/debug/shape/line.h"
#include "endstone/debug/shape/sphere.h"
#include "endstone/debug/shape/text.h"

namespace endstone {

using DebugShapeVariant = std::variant<DebugBox, DebugSphere, DebugCircle, DebugLine, DebugArrow, DebugText>;

}  // namespace endstone
