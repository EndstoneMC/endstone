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

enum class ChunkState : unsigned char {
    Unloaded = 0,
    Generating = 1,
    Generated = 2,
    PostProcessing = 3,
    PostProcessed = 4,
    CheckingForReplacementData = 5,
    NeedsLighting = 6,
    Lighting = 7,
    LightingFinished = 8,
    Loaded = 9,
    Invalid = 0xff,
};