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

namespace endstone::detail {

/**
 * @brief Cast a function pointer to void pointer
 *
 * @tparam Return The return type of the function.
 * @tparam Args The argument types of the function.
 *
 * @param fp The function pointer to be casted.
 *
 * @return A void pointer to the function.
 */
template <typename Return, typename... Args>
void *fp_cast(Return (*fp)(Args...))
{
    union {
        Return (*p)(Args...);
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}

/**
 * @brief Cast a member function pointer to void pointer.
 *
 * @tparam Return The return type of the member function.
 * @tparam Class The class type that the member function belongs to.
 * @tparam Args The argument types of the member function.
 *
 * @param fp Pointer to the member function to be casted.
 *
 * @return A void pointer to the member function.
 */
template <typename Return, typename Class, typename... Args>
void *fp_cast(Return (Class::*fp)(Args...))
{
    union {
        Return (Class::*p)(Args...);
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}

/**
 * @brief Cast a constant member function pointer to void pointer.
 *
 * @tparam Return The return type of the constant member function.
 * @tparam Class The class type that the constant member function belongs to.
 * @tparam Args The argument types of the constant member function.
 *
 * @param fp Pointer to the constant member function to be casted.
 *
 * @return A void pointer to the constant member function.
 */
template <typename Return, typename Class, typename... Args>
void *fp_cast(Return (Class::*fp)(Args...) const)
{
    union {
        Return (Class::*p)(Args...) const;
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}
}  // namespace endstone::detail
