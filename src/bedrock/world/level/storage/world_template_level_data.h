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

class WorldTemplateLevelData {
public:
    WorldTemplateLevelData();
    WorldTemplateLevelData(bool, bool, const BaseGameVersion &, const BaseGameVersion &);
    void getTagData(const CompoundTag &);
    void setTagData(CompoundTag &) const;
    [[nodiscard]] const ContentIdentity &getContentIdentity() const;
    void setContentIdentity(const ContentIdentity &);
    [[nodiscard]] const PackIdVersion &getWorldTemplateIdentity() const;
    void setWorldTemplateIdentity(const PackIdVersion &);
    [[nodiscard]] const BaseGameVersion &getBaseGameVersion() const;
    void setBaseGameVersion(const BaseGameVersion &);
    void setMaxBaseGameVersion(const BaseGameVersion &);
    [[nodiscard]] bool isFromWorldTemplate() const;
    void setIsFromWorldTemplate(bool);
    [[nodiscard]] bool isWorldTemplateOptionLocked() const;
    void setIsWorldTemplateOptionLocked(bool);

protected:
    void _getTagData(const CompoundTag &);
    void _setTagData(CompoundTag &) const;

private:
    ContentIdentity premium_template_content_identity_;
    PackIdVersion world_template_identity_;
    BaseGameVersion base_game_version_;
    BaseGameVersion max_base_game_version_;
    bool is_from_world_template_;
    bool is_world_template_option_locked_;
};
static_assert(sizeof(WorldTemplateLevelData) == 144);
