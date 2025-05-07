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

#include "endstone/server.h"

namespace endstone::core {

template <typename T>
class MinecraftRegistry {
public:
    const T *get(std::string_view key);
    std::vector<std::string_view> keys() const;
};

template <typename T, typename M>
class EndstoneRegistry final : public Registry<T> {
public:
    using MinecraftToEndstoneFunc = std::function<std::unique_ptr<T>(std::string_view, const M &)>;
    EndstoneRegistry(MinecraftRegistry<M> minecraft_registry, MinecraftToEndstoneFunc minecraft_to_endstone)
        : minecraft_registry_(std::move(minecraft_registry)), minecraft_to_endstone_(minecraft_to_endstone)
    {
    }

    T *get(typename Registry<T>::key_type key) noexcept override
    {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second.get();
        }

        auto endstone = createEndstone(key, minecraft_registry_.get(key));
        if (!endstone) {
            return nullptr;
        }

        T *raw = endstone.get();
        cache_.emplace(key, std::move(endstone));
        return raw;
    }

    const T *get(typename Registry<T>::key_type key) const noexcept override
    {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second.get();
        }

        auto endstone = createEndstone(key, minecraft_registry_.get(key));
        if (!endstone) {
            return nullptr;
        }

        T *raw = endstone.get();
        cache_.emplace(key, std::move(endstone));
        return raw;
    }

    T &getOrThrow(typename Registry<T>::key_type key) override
    {
        T *result = get(key);
        if (!result) {
            throw std::out_of_range{std::string{"EndstoneRegistry: key not found: "} + std::string(key)};
        }
        return *result;
    }

    const T &getOrThrow(typename Registry<T>::key_type key) const override
    {
        const T *result = get(key);
        if (!result) {
            throw std::out_of_range{std::string{"EndstoneRegistry: key not found: "} + std::string(key)};
        }
        return *result;
    }

private:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T *;
        using difference_type = std::ptrdiff_t;
        using pointer = T **;
        using reference = T *&;

        Iterator(EndstoneRegistry *owner, std::vector<std::string_view>::const_iterator key_it)
            : owner_(owner), key_it_(key_it)
        {
        }

        value_type operator*() const
        {
            return owner_->get(*key_it_);
        }

        Iterator &operator++()
        {
            ++key_it_;
            return *this;
        }

        bool operator==(const Iterator &other) const
        {
            return key_it_ == other.key_it_;
        }
        bool operator!=(const Iterator &other) const
        {
            return !(*this == other);
        }

    private:
        EndstoneRegistry *owner_;
        std::vector<std::string_view>::const_iterator key_it_;
    };

    class ConstIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T *;
        using difference_type = std::ptrdiff_t;
        using pointer = const T **;
        using reference = const T *&;

        ConstIterator(const EndstoneRegistry *owner, std::vector<std::string_view>::const_iterator key_it)
            : owner_(owner), key_it_(key_it)
        {
        }

        value_type operator*() const
        {
            return owner_->get(*key_it_);
        }

        ConstIterator &operator++()
        {
            ++key_it_;
            return *this;
        }

        bool operator==(const ConstIterator &other) const
        {
            return key_it_ == other.key_it_;
        }
        bool operator!=(const ConstIterator &other) const
        {
            return !(*this == other);
        }

    private:
        const EndstoneRegistry *owner_;
        std::vector<std::string_view>::const_iterator key_it_;
    };

public:
    Iterator begin() override
    {
        return Iterator{this, minecraft_registry_.keys().cbegin()};
    }

    Iterator end() override
    {
        return Iterator{this, minecraft_registry_.keys().cend()};
    }

    ConstIterator begin() const override
    {
        return ConstIterator{this, minecraft_registry_.keys().cbegin()};
    }

    ConstIterator end() const override
    {
        return ConstIterator{this, minecraft_registry_.keys().cend()};
    }

private:
    std::unique_ptr<T> createEndstone(std::string_view key, const M *minecraft)
    {
        if (minecraft == nullptr) {
            return nullptr;
        }
        return minecraft_to_endstone_(key, minecraft);
    }

    MinecraftRegistry<M> minecraft_registry_;
    mutable typename Registry<T>::storage_type cache_;
    MinecraftToEndstoneFunc minecraft_to_endstone_;
};

}  // namespace endstone::core
