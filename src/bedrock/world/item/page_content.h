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

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/compound_tag.h"

struct PageContent {
    static const PageContent BLANK_PAGE;
    enum class PageType : std::uint8_t {
        Text = 0,
        Photo = 1,
        Count = 2,
    };
    static PageContent read(const CompoundTag &tag);
    static void write(const PageContent &page, CompoundTag &tag);
    [[nodiscard]] std::unique_ptr<CompoundTag> createTag() const;

    std::string text;
    std::optional<std::string> filtered_text;
    std::optional<std::string> parsed_filtered_text;
    std::string parsed_text;
    std::string photo_name;
    std::optional<std::string> filtered_photo_name;
};
BEDROCK_STATIC_ASSERT_SIZE(PageContent, 216, 168);
