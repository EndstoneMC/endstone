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

#include "endstone/registry.h"
#include "endstone/server.h"

namespace endstone::core {
template <typename E, typename M>
class EndstoneRegistry final : public Registry<E> {
public:
    using MinecraftToEndstoneFunc = std::function<std::unique_ptr<E>(const std::string &, const M &)>;

    explicit EndstoneRegistry(MinecraftToEndstoneFunc minecraft_to_endstone)
        : minecraft_to_endstone_(minecraft_to_endstone)
    {
    }

    E *get(Identifier<E> id) noexcept override
    {
        auto it = cache_.find(id);
        if (it != cache_.end()) {
            return it->second.get();
        }

        auto endstone = create(id, getMinecraft(id));
        if (!endstone) {
            return nullptr;
        }

        E *raw = endstone.get();
        cache_.emplace(id, std::move(endstone));
        return raw;
    }

    const E *get(Identifier<E> id) const noexcept override
    {
        auto it = cache_.find(id);
        if (it != cache_.end()) {
            return it->second.get();
        }

        auto endstone = create(id, getMinecraft(id));
        if (!endstone) {
            return nullptr;
        }

        E *raw = endstone.get();
        cache_.emplace(id, std::move(endstone));
        return raw;
    }

    E &getOrThrow(Identifier<E> id) override
    {
        E *result = get(id);
        if (!result) {
            throw std::out_of_range{fmt::format("EndstoneRegistry: identifier not found: {}", id)};
        }
        return *result;
    }

    const E &getOrThrow(Identifier<E> id) const override
    {
        const E *result = get(id);
        if (!result) {
            throw std::out_of_range{fmt::format("EndstoneRegistry: identifier not found: {}", id)};
        }
        return *result;
    }

    void forEach(std::function<bool(const E &)> func) const override
    {
        for (const auto &identifier : identifiers()) {
            if (!func(getOrThrow(identifier))) {
                break;
            }
        }
    }

    [[nodiscard]] std::vector<Identifier<E>> identifiers() const;
    static std::unique_ptr<Registry<E>> create();

private:
    std::unique_ptr<E> create(Identifier<E> id, const M *minecraft) const
    {
        if (minecraft == nullptr) {
            return nullptr;
        }
        return minecraft_to_endstone_(id, *minecraft);
    }

    [[nodiscard]] const M *getMinecraft(Identifier<E> id) const;

    mutable std::unordered_map<std::string, std::unique_ptr<E>> cache_;
    MinecraftToEndstoneFunc minecraft_to_endstone_;
};

}  // namespace endstone::core
