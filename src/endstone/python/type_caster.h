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
    explicit type_caster() : value(0, 0) {}

    // Python -> C++ conversion
    bool load(handle src, bool)
    {
        // Expect a numpy array with shape (H, W, 4) and dtype=uint8
        array_t<uint8_t, array::c_style | array::forcecast> array;
        try {
            array = pybind11::cast<array_t<uint8_t, array::c_style | array::forcecast>>(src);
        }
        catch (const cast_error &) {
            PyErr_SetString(PyExc_TypeError, "Expected a NumPy array with dtype=uint8 and C-style memory layout");
            return false;
        }

        if (array.ndim() != 3 || array.shape(2) != 4) {
            PyErr_SetString(PyExc_ValueError, "NumPy array must have 3 dimensions and 4 channels (shape: H x W x 4)");
            return false;
        }

        const int height = static_cast<int>(array.shape(0));
        const int width = static_cast<int>(array.shape(1));

        auto result = endstone::Image::fromBuffer(
            width, height, std::string_view(reinterpret_cast<const char *>(array.data()), height * width * 4));
        if (!result) {
            PyErr_SetString(PyExc_ValueError, result.error().c_str());
            return false;
        }
        value = std::move(result.value());
        return PyErr_Occurred() == nullptr;
    }

    // C++ -> Python conversion
    static handle cast(const endstone::Image &img, return_value_policy /* policy */, handle /* parent */)
    {
        const int width = img.getWidth();
        const int height = img.getHeight();

        // Create a numpy array with shape (H, W, 4)
        std::vector<ssize_t> shape = {height, width, 4};
        std::vector<ssize_t> strides = {width * 4, 4, 1};

        // Allocate a new array and copy
        array_t<uint8_t> array(buffer_info(const_cast<char *>(img.getData().data()), /* data */
                                           1,                                        /* size of one scalar */
                                           format_descriptor<uint8_t>::format(),     /* data type */
                                           3,                                        /* number of dimensions */
                                           shape,                                    /* shape */
                                           strides                                   /* strides */
                                           ));

        // Make an owned copy so buffer can own it
        return array.attr("copy")().release();
    }
    PYBIND11_TYPE_CASTER(endstone::Image, const_name("numpy.ndarray[numpy.uint8]"));
};

}  // namespace pybind11::detail
