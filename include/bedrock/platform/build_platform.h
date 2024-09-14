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

enum class BuildPlatform {
    Unknown = -1,
    Google = 1,
    iOS = 2,
    OSX = 3,
    Amazon = 4,
    GearVR = 5,
    UWP = 7,
    Win32 = 8,
    Dedicated = 9,
    tvOS_Deprecated = 10,
    Sony = 11,
    Nx = 12,
    Xbox = 13,
    WindowsPhone_Deprecated = 14,
    Linux = 15,
};
