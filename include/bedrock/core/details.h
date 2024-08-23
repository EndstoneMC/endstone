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

namespace Details {

template <typename T>
class ValueOrRef {
public:
    explicit ValueOrRef(T const &ref)
    {
        index_ = 0;
        storage_.value = ref;
    }

    ~ValueOrRef()
    {
        if (index_ == 0) {
            storage_.value.~T();
        }
    }

    T asValue() const noexcept
    {
        switch (index_) {
        case 0:
            return storage_.value;
        case 1:
            return *storage_.ref;
        default:
            return T();
        }
    }

private:
    union Storage {
        T value;
        T *ref;
        Storage() {}
        ~Storage() {}
    } storage_;
    std::int8_t index_{-1};
};

}  // namespace Details
