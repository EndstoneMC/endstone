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

#include "bedrock/core/resource/resource_helper.h"

ResourceLocation::ResourceLocation()
{
    _computeHashes();
}

ResourceLocation::ResourceLocation(Core::Path const &path) : path_(path)
{
    _computeHashes();
}

ResourceLocation::ResourceLocation(Core::Path const &path, ResourceFileSystem file_system)
    : path_(path), file_system(file_system)
{
    _computeHashes();
}

Core::HeapPathBuffer const &ResourceLocation::getRelativePath() const
{
    return path_;
}

void ResourceLocation::_computeHashes()
{
    constexpr HashType64 offset_basis = 0xCBF29CE484222325;
    constexpr HashType64 prime = 0x100000001B3;

    path_hash_ = HashedString::computeHash(path_.getContainer().c_str());
    full_hash_ = path_hash_ ^ (prime * (static_cast<std::uint8_t>(file_system) ^ offset_basis));
}
