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

namespace Bedrock {
template <typename TWrappedPtr, typename TEnum>
class DenseEnumMapIterator {
    using difference_type = std::iterator_traits<TWrappedPtr>::difference_type;
    using key_type = TEnum;
    using reference = std::pair<const TEnum, typename std::iterator_traits<TWrappedPtr>::reference>;
    // using pointer = Bedrock::ArrowProxy<reference>;

public:
    DenseEnumMapIterator();
    DenseEnumMapIterator(TWrappedPtr, const difference_type);
    reference operator*();
    // pointer operator->();
    reference operator[](const difference_type);
    DenseEnumMapIterator &operator++();
    DenseEnumMapIterator operator++(int);
    DenseEnumMapIterator &operator--();
    DenseEnumMapIterator operator--(int);
    DenseEnumMapIterator &operator+=(const difference_type);
    DenseEnumMapIterator &operator-=(const difference_type);

private:
    TWrappedPtr data_head_;
    difference_type index_;
};

template <typename TEnum, typename TValue, TEnum SizeEnum>
class DenseEnumMap {
    static constexpr size_t MaxSize = static_cast<std::underlying_type_t<TEnum>>(SizeEnum);
    using container_type = std::array<TValue, MaxSize>;
    using key_type = TEnum;
    using size_type = std::array<TValue, MaxSize>::size_type;
    using reference = std::array<TValue, MaxSize>::reference;
    using const_reference = std::array<TValue, MaxSize>::const_reference;
    using iterator = DenseEnumMapIterator<TValue *, TEnum>;
    using const_iterator = DenseEnumMapIterator<const TValue *, TEnum>;

public:
    DenseEnumMap();
    explicit DenseEnumMap(const_reference);
    DenseEnumMap(const std::initializer_list<std::pair<TEnum, TValue>>);
    reference operator[](const key_type);
    const_reference operator[](const key_type) const;
    reference indexDirect(const size_type);
    const_reference indexDirect(const size_type) const;
    void fill(const_reference);
    void swap(DenseEnumMap &);
    gsl::span<int> asSpan();
    gsl::span<const int> asSpan() const;
    size_type size();
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

private:
    container_type container_;
};
}  // namespace Bedrock
