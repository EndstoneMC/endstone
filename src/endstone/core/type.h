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

#include <typeinfo>

#include "endstone/object.h"

namespace endstone::core {

/**
 * @brief Registers the endstone type hierarchy with entt::meta.
 *
 * Must be called once at server startup before any use of Object::as<T>() or Object::is<T>().
 */
void registerTypes();

/**
 * @brief Checks if the type identified by `from` is an instance of (or subclass of) `target`
 *        in the registered type hierarchy.
 *
 * @param from The runtime type of the object.
 * @param target The type to check against.
 * @return True if `from` is `target` or derives from `target`.
 */
bool isTypeInstanceOf(const std::type_info &from, const std::type_info &target);

/**
 * @brief Checks if the given object is an instance of (or subclass of) `target`.
 */
bool isInstanceOf(const Object &obj, const std::type_info &target);

}  // namespace endstone::core
