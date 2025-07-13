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

#include <gsl/gsl>

namespace mce {
class Blob {
    using value_type = uint8_t;
    using delete_function = void (*)(value_type *);
    static value_type *defaultAllocator(size_t);
    static void defaultDeleter(value_type *);

public:
    struct Deleter {
        Deleter();
        Deleter(delete_function);
        void operator()(value_type *) const;
        delete_function getDeleteFunc() const;
        delete_function func;
    };
    static_assert(sizeof(Deleter) == 8);

private:
    using pointer_type = std::unique_ptr<unsigned char[], Deleter>;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = value_type *;
    using const_iterator = const value_type *;
    using size_type = size_t;

public:
    Blob();
    Blob(const Blob &) = delete;
    Blob(Blob &&);
    Blob(const size_type);
    Blob(const value_type *const, const size_type);
    Blob(pointer_type &&, const size_type);
    Blob &operator=(const Blob &) = delete;
    Blob &operator=(Blob &&);
    ~Blob();
    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;
    void swap(Blob &);
    size_type size() const
    {
        return size_;
    }
    size_type max_size() const;
    bool empty() const
    {
        return size() == 0;
    }
    value_type *data()
    {
        return blob_.get();
    }
    const value_type *data() const
    {
        return blob_.get();
    }
    value_type *get();
    const value_type *get() const;
    gsl::span<unsigned char> getSpan();
    gsl::span<const unsigned char> getSpan() const;
    pointer_type release();
    void fillBlob(const uint8_t);

private:
    pointer_type blob_;
    size_type size_;
};
static_assert(sizeof(Blob) == 24);
}  // namespace mce
