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

enum class PermissibleRole {
    Player,
    Operator,
    Owner
};

class Plugin;
class PermissionAttachment;

class Permissible {
public:
    Permissible() noexcept = default;
    virtual ~Permissible() noexcept = default;

    // Delete copy constructor and copy assignment operator
    Permissible(const Permissible &) = delete;
    Permissible &operator=(const Permissible &) = delete;

    [[nodiscard]] virtual PermissibleRole getRole() const noexcept = 0;
    [[nodiscard]] virtual bool setRole(PermissibleRole role) const noexcept = 0;
    [[nodiscard]] virtual bool isPermissionSet(const std::string &name) const noexcept = 0;
    [[nodiscard]] virtual bool hasPermission(const std::string &name) const noexcept = 0;
    [[nodiscard]] virtual PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name,
                                                              bool value) noexcept = 0;
    [[nodiscard]] virtual PermissionAttachment *addAttachment(Plugin &plugin) noexcept = 0;
    [[nodiscard]] virtual bool removeAttachment(const PermissionAttachment &attachment) noexcept = 0;
    virtual void recalculatePermissions() noexcept = 0;
};
