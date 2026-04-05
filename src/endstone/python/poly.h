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

#include <pybind11/pybind11.h>

namespace PYBIND11_NAMESPACE {

/**
 * @brief Generic polymorphic type hook for all endstone::Object-derived types.
 *
 * Uses Object::getClassTypeId() to report the public API type to pybind11,
 * enabling correct isinstance() dispatch without per-type hooks.
 */
template <typename T>
struct polymorphic_type_hook<T, std::enable_if_t<std::is_base_of_v<endstone::Object, T>>> {
    static const void *get(const T *src, const std::type_info *&type)
    {
        if (!src) {
            return src;
        }
        type = &src->getClassTypeId();
        return src;
    }
};

}  // namespace PYBIND11_NAMESPACE
