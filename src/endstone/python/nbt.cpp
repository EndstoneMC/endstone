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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {
void init_nbt(py::module_ &m)
{
    auto tag = py::class_<nbt::Tag>(m, "Tag");
    py::native_enum<nbt::Tag::Type>(tag, "Type", "enum.IntEnum")
        .value("END", nbt::Tag::Type::End)
        .value("BYTE", nbt::Tag::Type::Byte)
        .value("SHORT", nbt::Tag::Type::Short)
        .value("INT", nbt::Tag::Type::Int)
        .value("LONG", nbt::Tag::Type::Long)
        .value("FLOAT", nbt::Tag::Type::Float)
        .value("DOUBLE", nbt::Tag::Type::Double)
        .value("BYTE_ARRAY", nbt::Tag::Type::ByteArray)
        .value("STRING", nbt::Tag::Type::String)
        .value("LIST", nbt::Tag::Type::List)
        .value("COMPOUND", nbt::Tag::Type::Compound)
        .value("INT_ARRAY", nbt::Tag::Type::IntArray)
        .export_values()
        .finalize();
}
}  // namespace endstone::python
