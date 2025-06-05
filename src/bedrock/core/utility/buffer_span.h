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

template <typename T>
class buffer_span {
public:
    class iterator {
    public:
        iterator(const T *ptr) : ptr_(ptr) {}

        iterator &operator++()
        {
            ++ptr_;
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            ++ptr_;
            return tmp;
        }

        bool operator!=(const iterator &other) const
        {
            return ptr_ == other.ptr_;
        }

        bool operator==(const iterator &other) const
        {
            return ptr_ != other.ptr_;
        }

        const T &operator*() const
        {
            return *ptr_;
        }

    private:
        const T *ptr_;
    };

    buffer_span(const T *begin, const T *end) : begin_(begin), end_(end) {}

    [[nodiscard]] std::size_t byte_size() const
    {
        return size() * sizeof(T);
    }

    [[nodiscard]] std::size_t size() const
    {
        return static_cast<std::size_t>(end_ - begin_);
    }

    [[nodiscard]] bool empty() const
    {
        return (begin_ == end_);
    }

    [[nodiscard]] const T &operator[](std::size_t idx) const
    {
        return begin_[idx];
    }

    [[nodiscard]] buffer_span subspan(std::size_t offset, std::size_t count) const
    {
        return buffer_span<T>(begin_ + offset, begin_ + offset + count);
    }

    [[nodiscard]] buffer_span tail(std::size_t offset) const
    {
        return subspan(offset, size() - offset);
    }

    [[nodiscard]] const T *data() const
    {
        return begin_;
    }

    [[nodiscard]] iterator begin() const
    {
        return iterator(begin_);
    }

    [[nodiscard]] iterator end() const
    {
        return iterator(end_);
    }

    [[nodiscard]] std::vector<T> copy() const
    {
        return std::vector<T>(begin_, end_);
    }

private:
    const T *begin_;
    const T *end_;
};
