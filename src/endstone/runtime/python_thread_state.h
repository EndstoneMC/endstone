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

namespace endstone::runtime::python {

class PythonThreadContext {
public:
    PythonThreadContext() = default;

    PythonThreadContext(const PythonThreadContext &) = delete;
    PythonThreadContext &operator=(const PythonThreadContext &) = delete;

private:
    // release_ restores the GIL before acquire_ releases the thread state.
    pybind11::gil_scoped_acquire acquire_;
    pybind11::gil_scoped_release release_;
};

}  // namespace endstone::runtime::python
