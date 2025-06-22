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

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "endstone/endstone.hpp"

namespace pybind11::detail {
template <>
class type_caster<endstone::UUID> {
public:
    // Python -> C++
    bool load(handle src, bool)
    {
        /* Extract PyObject from handle */
        PyObject *source = src.ptr();

        /* Check if the Python Object is a UUID instance */
        PyObject *uuid_module = PyImport_ImportModule("uuid");
        PyObject *uuid_class = PyObject_GetAttrString(uuid_module, "UUID");
        if (!PyObject_IsInstance(source, uuid_class)) {
            PyErr_SetString(PyExc_TypeError, "Object is not an instance of UUID");
            Py_XDECREF(uuid_module);
            Py_XDECREF(uuid_class);
            return false;
        }

        /* Now try to convert into C++ object */
        PyObject *bytes = PyObject_GetAttrString(source, "bytes");
        if (PyBytes_GET_SIZE(bytes) != 16) {
            PyErr_SetString(PyExc_ValueError, "UUID bytes size must be 16");
            Py_XDECREF(bytes);
            Py_XDECREF(uuid_module);
            Py_XDECREF(uuid_class);
            return false;
        }

        const char *bytes_data = PyBytes_AS_STRING(bytes);
        for (int i = 0; i < 16; i++) {
            value.data[i] = bytes_data[i];
        }

        /* Clean up */
        Py_XDECREF(bytes);
        Py_XDECREF(uuid_module);
        Py_XDECREF(uuid_class);
        return PyErr_Occurred() == nullptr;
    }

    // C++ -> Python
    static handle cast(endstone::UUID src, return_value_policy /* policy */, handle /* parent */)
    {
        PyObject *uuid_module = PyImport_ImportModule("uuid");
        PyObject *uuid_class = PyObject_GetAttrString(uuid_module, "UUID");

        // Create Python bytes object from the UUID struct data.
        PyObject *py_bytes = PyBytes_FromStringAndSize(reinterpret_cast<const char *>(src.data), 16);

        // Create a dictionary to hold keyword arguments for the UUID constructor.
        PyObject *kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "bytes", py_bytes);

        PyObject *res = PyObject_Call(uuid_class, PyTuple_New(0), kwargs);

        Py_XDECREF(py_bytes);
        Py_XDECREF(kwargs);
        Py_XDECREF(uuid_module);
        Py_XDECREF(uuid_class);

        return {res};
    }

    PYBIND11_TYPE_CASTER(endstone::UUID, const_name("uuid.UUID"));
};

template <typename Value>
class type_caster<endstone::Result<Value>> {
public:
    using value_conv = make_caster<Value>;

    template <typename T>
    static handle cast(T &&src, return_value_policy policy, handle parent)
    {
        if (!src) {
            throw std::runtime_error(src.error());
        }
        if (!std::is_lvalue_reference<T>::value) {
            policy = return_value_policy_override<Value>::policy(policy);
        }
        return value_conv::cast(*std::forward<T>(src), policy, parent);
    }
    PYBIND11_TYPE_CASTER(endstone::Result<Value>, value_conv::name);
};

template <>
class type_caster<endstone::Result<void>> {
public:
    template <typename T>
    static handle cast(T &&src, return_value_policy policy, handle parent)
    {
        if (!src) {
            throw std::runtime_error(src.error());
        }
        return none().release();
    }
    PYBIND11_TYPE_CASTER(endstone::Result<void>, const_name("None"));
};

// Type caster for endstone::Image <-> numpy.ndarray of uint8 RGBA
template <>
class type_caster<endstone::Image> {
public:
    explicit type_caster() : value(endstone::Image::Type::Invalid, 0, 0) {}

    // Convert from Python -> C++
    bool load(handle src, bool)
    {
        // Ensure the object is a NumPy array of uint8
        auto array = pybind11::array_t<uint8_t, pybind11::array::c_style | pybind11::array::forcecast>::ensure(src);
        if (!array) {
            PyErr_SetString(PyExc_TypeError, "TypeError: expected a numpy.ndarray of uint8");
            return false;
        }

        int ndim = array.ndim();
        int height, width, depth;
        endstone::Image::Type img_type;

        if (ndim == 2) {
            // Grayscale image: shape = (height, width)
            height = static_cast<int>(array.shape(0));
            width = static_cast<int>(array.shape(1));
            depth = 1;
            img_type = endstone::Image::Type::Grayscale;
        }
        else if (ndim == 3) {
            // RGB or RGBA image: shape = (height, width, depth)
            height = static_cast<int>(array.shape(0));
            width = static_cast<int>(array.shape(1));
            depth = static_cast<int>(array.shape(2));
            if (depth == 3) {
                img_type = endstone::Image::Type::RGB;
            }
            else if (depth == 4) {
                img_type = endstone::Image::Type::RGBA;
            }
            else {
                PyErr_SetString(PyExc_TypeError, "TypeError: expected the last dimension to be 3 (RGB) or 4 (RGBA)");
                return false;
            }
        }
        else {
            // Unsupported number of dimensions
            PyErr_SetString(PyExc_TypeError, "TypeError: expected a 2D (grayscale) or 3D (RGB/RGBA) array");
            return false;
        }

        // Compute expected buffer size
        size_t expected_size = static_cast<size_t>(height) * width * depth;
        // Pointer to the raw data in the NumPy array
        uint8_t *data_ptr = reinterpret_cast<uint8_t *>(array.mutable_data());

        // Copy into a std::string
        std::string buffer(reinterpret_cast<char *>(data_ptr), expected_size);

        // Construct an Image from the byte buffer
        auto result = endstone::Image::fromBuffer(img_type, width, height, buffer);
        if (!result) {
            PyErr_SetString(PyExc_TypeError, "TypeError: failed to construct Image from buffer");
            return false;
        }
        // Move the constructed Image into the caster's value
        value = std::move(result.value());
        return true;
    }

    // Convert from C++ -> Python
    static handle cast(endstone::Image src, return_value_policy /* policy */, handle /* parent */)
    {
        int width = src.getWidth();
        int height = src.getHeight();
        int depth = src.getDepth();

        // Decide on the NumPy shape
        std::vector<ssize_t> shape;
        if (depth == 1) {
            // Grayscale: 2D array
            shape = {static_cast<ssize_t>(height), static_cast<ssize_t>(width)};
        }
        else {
            // RGB or RGBA: 3D array
            shape = {static_cast<ssize_t>(height), static_cast<ssize_t>(width), static_cast<ssize_t>(depth)};
        }

        // Allocate a new NumPy array of type uint8
        auto array = pybind11::array_t<uint8_t>(shape);
        // Copy the pixel data from the Image into the NumPy buffer
        uint8_t *buffer_ptr = array.mutable_data();
        auto data_view = src.getData();  // std::string_view of the underlying bytes
        std::memcpy(buffer_ptr, data_view.data(), data_view.size());

        return array.release();
    }

    PYBIND11_TYPE_CASTER(endstone::Image, const_name("numpy.ndarray[numpy.uint8]"));
};

template <>
class type_caster<endstone::Color> {
public:
    explicit type_caster() : value(0, 0, 0) {}

    // Python -> C++
    bool load(handle src, bool)
    {
        if (!pybind11::isinstance<sequence>(src)) {
            PyErr_SetString(PyExc_ValueError, "Color must be a sequence of 3 or 4 integers");
            return false;
        }

        auto seq = reinterpret_borrow<sequence>(src);
        size_t len = seq.size();

        if (len != 3 && len != 4) {
            PyErr_SetString(PyExc_ValueError, "Color tuple must have length 3 or 4");
            return false;
        }

        // Cast elements to int
        try {
            auto result =
                endstone::Color::fromRGBA(seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(), seq[2].cast<uint8_t>(),
                                          static_cast<uint8_t>(len == 4 ? seq[3].cast<uint8_t>() : 255));
            if (!result) {
                PyErr_SetString(PyExc_ValueError, result.error().c_str());
                return false;
            }
            value = std::move(result.value());
        }
        catch (const cast_error &) {
            PyErr_SetString(PyExc_ValueError, "Color elements must be integers");
            return false;
        }
        return true;
    }

    // C++ -> Python
    static handle cast(const endstone::Color &src, return_value_policy /* policy */, handle /* parent */)
    {
        return make_tuple(src.getRed(), src.getGreen(), src.getBlue(), src.getAlpha()).release();
    }

    PYBIND11_TYPE_CASTER(endstone::Color, const_name("tuple[int, ...]"));
};

}  // namespace pybind11::detail
