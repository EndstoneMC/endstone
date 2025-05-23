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

#include "endstone/namespaced_key.h"
#include "endstone/registry.h"
#include "endstone/server.h"

namespace endstone::core {

template <typename T>
class MinecraftRegistry {
public:
    [[nodiscard]] const T *get(const NamespacedKey &key) const;
    [[nodiscard]] std::vector<NamespacedKey> keys() const;
};

template <typename E, typename M>
class EndstoneRegistry final : public Registry<E> {
public:
    using MinecraftToEndstoneFunc = std::function<std::unique_ptr<E>(NamespacedKey, const M &)>;

    explicit EndstoneRegistry(MinecraftToEndstoneFunc minecraft_to_endstone)
        : minecraft_to_endstone_(minecraft_to_endstone)
    {
    }

    E *get(NamespacedKey key) noexcept override
    {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second.get();
        }

        auto endstone = createEndstone(key, minecraft_registry_.get(key));
        if (!endstone) {
            return nullptr;
        }

        E *raw = endstone.get();
        cache_.emplace(key, std::move(endstone));
        return raw;
    }

    const E *get(NamespacedKey key) const noexcept override
    {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second.get();
        }

        auto endstone = createEndstone(key, minecraft_registry_.get(key));
        if (!endstone) {
            return nullptr;
        }

        E *raw = endstone.get();
        cache_.emplace(key, std::move(endstone));
        return raw;
    }

    E &getOrThrow(const NamespacedKey key) override
    {
        E *result = get(key);
        if (!result) {
            throw std::out_of_range{std::string{"EndstoneRegistry: key not found: "} + key.toString()};
        }
        return *result;
    }

    const E &getOrThrow(const NamespacedKey key) const override
    {
        const E *result = get(key);
        if (!result) {
            throw std::out_of_range{std::string{"EndstoneRegistry: key not found: "} + key.toString()};
        }
        return *result;
    }

    void forEach(std::function<bool(const E &)> func) const override
    {
        for (const auto &key : minecraft_registry_.keys()) {
            if (!func(getOrThrow(key))) {
                break;
            }
        }
    }

    static std::unique_ptr<Registry<E>> createRegistry();

private:
    std::unique_ptr<E> createEndstone(NamespacedKey key, const M *minecraft) const
    {
        if (minecraft == nullptr) {
            return nullptr;
        }
        return minecraft_to_endstone_(key, *minecraft);
    }

    MinecraftRegistry<M> minecraft_registry_;
    mutable std::unordered_map<NamespacedKey, std::unique_ptr<E>> cache_;
    MinecraftToEndstoneFunc minecraft_to_endstone_;
};

}  // namespace endstone::core
