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
class optional_ref {
public:
    optional_ref(nullptr_t) : ptr_(nullptr) {}

    [[nodiscard]] T &unwrap() const
    {
        return *ptr_;
    }

    [[nodiscard]] bool is_some() const
    {
        return ptr_ != nullptr;
    }

    [[nodiscard]] bool is_none() const
    {
        return ptr_ == nullptr;
    }

protected:
    T *ptr_;
};
