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
#include "literal.h"

namespace py = pybind11;

namespace endstone::python {
static py::object tag_to_python(const nbt::Tag &tag)
{
    return tag.visit([](auto &&arg) -> py::object {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return py::none();
        }
        else if constexpr (std::is_same_v<T, ByteTag> || std::is_same_v<T, ShortTag> || std::is_same_v<T, IntTag> ||
                           std::is_same_v<T, LongTag>) {
            return py::int_(arg.value());
        }
        else if constexpr (std::is_same_v<T, FloatTag> || std::is_same_v<T, DoubleTag>) {
            return py::float_(arg.value());
        }
        else if constexpr (std::is_same_v<T, StringTag>) {
            return py::str(arg.value());
        }
        else if constexpr (std::is_same_v<T, ByteArrayTag>) {
            const auto *ptr = reinterpret_cast<const char *>(arg.data());
            return py::bytes(ptr, arg.size());
        }
        else if constexpr (std::is_same_v<T, IntArrayTag>) {
            py::list lst;
            for (const auto &elem : arg) {
                lst.append(elem);
            }
            return lst;
        }
        else if constexpr (std::is_same_v<T, ListTag>) {
            py::list lst;
            for (const auto &elem : arg) {
                lst.append(tag_to_python(elem));
            }
            return lst;
        }
        else if constexpr (std::is_same_v<T, CompoundTag>) {
            py::dict d;
            for (const auto &[k, v] : arg) {
                d[py::str(k)] = tag_to_python(v);
            }
            return d;
        }
        else {
            return py::none();
        }
    });
}

static std::size_t normalize_index(py::ssize_t i, std::size_t n)
{
    if (i < 0) {
        i += static_cast<py::ssize_t>(n);
    }
    if (i < 0 || static_cast<std::size_t>(i) >= n) {
        throw py::index_error("ListTag index out of range");
    }
    return static_cast<std::size_t>(i);
}

template <typename T, typename PyClass>
static void bind_dump(PyClass &cls)
{
    cls.def(
        "dump",
        [](const T &self, std::optional<std::string> name, const Literal<"little", "big"> &byte_order,
           bool network) -> py::bytes {
            nbt::Tag tag{self};
            auto endian = byte_order.value() == "big" ? std::endian::big : std::endian::little;
            std::string result;
            if (name.has_value()) {
                result = nbt::dump(tag, name.value(), endian, network);
            }
            else {
                result = nbt::dump(tag, endian, network);
            }
            return py::bytes(result);
        },
        py::arg("name") = py::none(), py::arg("byte_order") = "little", py::arg("network") = false, R"doc(
    Serialize this tag to binary NBT format.

    Args:
        name: Optional root tag name. If None, an empty name is used.
        byte_order: Byte order for encoding, either ``'little'`` (Bedrock) or ``'big'`` (Java).
        network: If True, use Bedrock network varint encoding for lengths and Int/Long values.

    Returns:
        bytes: The binary NBT data.
)doc");
}

template <typename T, typename... Args>
auto bind_value_tag(py::module &m, const char *name, Args &&...args)
{
    using value_type = T::value_type;
    auto cls = py::class_<T, nbt::TagBase>(m, name, std::forward<Args>(args)...)
                   .def(py::init<>())
                   .def(py::init<const value_type &>(), py::arg("value"))
                   .def_property_readonly("value", &T::value, "The underlying value of this tag.")
                   .def(py::self == py::self)
                   .def(py::self != py::self)
                   .def(py::self == value_type())
                   .def(py::self != value_type())
                   .def(value_type() == py::self)
                   .def(value_type() != py::self)
                   .def("__str__", [](const T &self) { return std::format("{}", self); })
                   .def("__repr__", [](const py::object &self) {
                       auto tag_name = py::type::of(self).attr("__name__").cast<std::string>();
                       return py::str("{}({})").format(tag_name, py::repr(self.attr("value")));
                   });

    if constexpr (std::is_integral_v<value_type>) {
        cls.def("__int__", [](const T &self) { return static_cast<py::int_>(self.value()); });
    }
    if constexpr (std::is_floating_point_v<value_type>) {
        cls.def("__float__", [](const T &self) { return static_cast<py::float_>(self.value()); });
    }
    bind_dump<T>(cls);
    return cls;
}

template <typename T, typename... Args>
static void bind_array_tag(py::module_ &m, const char *name, Args &&...args)
{
    auto cls = py::class_<T, nbt::TagBase>(m, name, std::forward<Args>(args)...);
    using value_type = T::value_type;
    using storage_type = T::storage_type;

    // --- Constructors ---
    cls.def(py::init<>())
        // From any iterable: ArrayTag([1,2,3])
        .def(py::init([](const py::iterable &it) {
                 storage_type v;
                 for (py::handle h : it) {
                     v.push_back(py::cast<value_type>(h));
                 }
                 return T(std::move(v));
             }),
             py::arg("iterable"));

    // Byte-array friendly ctor from Python buffer (bytes/bytearray/memoryview/numpy)
    if constexpr (std::is_same_v<value_type, std::int8_t> || std::is_same_v<value_type, std::uint8_t>) {
        cls.def(py::init([](py::buffer b) {
                    py::buffer_info info = b.request();
                    if (info.ndim != 1) {
                        throw py::value_error("buffer must be 1-D");
                    }
                    // Accept any 1-byte itemsize buffer; reinterpret as bytes
                    if (info.itemsize != 1) {
                        throw py::value_error("buffer itemsize must be 1");
                    }
                    storage_type v(static_cast<size_t>(info.size));
                    std::memcpy(v.data(), info.ptr, v.size());
                    return T(std::move(v));
                }),
                py::arg("buffer"));
    }

    // --- Basic container methods ---
    cls.def("clear", &T::clear, "Removes all values from this tag.")
        .def("append", &T::push_back, py::arg("value"), R"doc(
    Appends a value to the end of this tag.

    Args:
        value: The value to append.
)doc")
        .def(
            "extend",
            [](T &self, const py::iterable &it) {
                for (py::handle h : it) {
                    self.push_back(py::cast<value_type>(h));
                }
            },
            py::arg("iterable"), R"doc(
    Appends all values from an iterable to the end of this tag.

    Args:
        iterable: An iterable of values to append.
)doc");

    // --- Python sequence protocol ---
    cls.def("__len__", [](const T &self) { return self.size(); })
        .def("__bool__", [](const T &self) { return !self.empty(); })
        .def("__getitem__",
             [=](const T &self, py::ssize_t i) -> value_type {
                 auto n = self.size();
                 return self[normalize_index(i, n)];
             })
        .def("__setitem__",
             [=](T &self, py::ssize_t i, const value_type &val) {
                 auto n = self.size();
                 self[normalize_index(i, n)] = val;
             })
        .def("__iter__", [](T &self) { return py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>());

    // --- Comparisons via operators.h ---
    cls.def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self == storage_type())
        .def(py::self != storage_type())
        .def(storage_type() == py::self)
        .def(storage_type() != py::self);

    // --- __repr__ ---
    cls.def("__str__", [](const T &self) { return std::format("{}", self); });
    cls.def("__repr__", [](const py::object &self) {
        auto tag_name = py::type::of(self).attr("__name__").cast<std::string>();
        py::list lst;
        for (const auto &value : self) {
            lst.append(value);
        }
        return py::str("{}({})").format(tag_name, py::repr(lst));
    });

    // --- Byte-array niceties: bytes() and buffer protocol ---
    if constexpr (std::is_same_v<value_type, std::int8_t> || std::is_same_v<value_type, std::uint8_t>) {
        cls.def("__bytes__", [](const T &self) {
            const auto *ptr = reinterpret_cast<const char *>(self.data());
            return py::bytes(ptr, self.size());
        });
        cls.def_buffer([](T &self) -> py::buffer_info {
            return py::buffer_info(self.data(),                                    // ptr
                                   sizeof(value_type),                             // itemsize
                                   py::format_descriptor<value_type>::format(),    // format
                                   1,                                              // ndim
                                   {static_cast<py::ssize_t>(self.size())},        // shape
                                   {static_cast<py::ssize_t>(sizeof(value_type))}  // strides
            );
        });
    }
    bind_dump<T>(cls);
}

static void bind_list_tag(py::module &m)
{
    auto cls = py::class_<ListTag, nbt::TagBase>(m, "ListTag", "An NBT list tag of homogeneously-typed tags.")
                   .def(py::init<>())
                   .def(py::init([](py::iterable iter) {
                            ListTag lt;
                            for (py::handle h : iter) {
                                auto v = py::cast<nbt::Tag>(h);
                                lt.emplace_back(v);
                            }
                            return lt;
                        }),
                        py::arg("iterable"))
                   .def("__len__", [](const ListTag &self) { return static_cast<py::ssize_t>(self.size()); })
                   .def("__bool__", [](const ListTag &self) { return !self.empty(); })
                   .def("__str__", [](const ListTag &self) { return std::format("{}", self); })
                   .def("__repr__",
                        [](const ListTag &self) {
                            py::list lst;
                            for (const auto &tag : self) {
                                lst.append(py::cast(tag));
                            }
                            return py::str("ListTag({})").format(py::repr(lst));
                        })
                   .def(
                       "__getitem__",
                       [](ListTag &self, py::ssize_t i) -> nbt::Tag & {
                           auto idx = normalize_index(i, self.size());
                           return self.at(idx);
                       },
                       py::return_value_policy::reference_internal)
                   .def("__setitem__",
                        [](ListTag &self, py::ssize_t i, const nbt::Tag &value) {
                            if (value.type() == nbt::Type::End) {
                                throw std::invalid_argument("ListTag cannot contain End tags.");
                            }
                            const auto n = self.size();
                            auto idx = normalize_index(i, n);
                            if (self.type() != nbt::Type::End && value.type() != self.type()) {
                                throw std::invalid_argument("ListTag elements must have the same type.");
                            }
                            self.at(idx) = value;
                        })
                   .def("__delitem__",
                        [](ListTag &self, py::ssize_t i) {
                            auto idx = normalize_index(i, self.size());
                            self.erase(self.cbegin() + static_cast<std::ptrdiff_t>(idx));
                        })
                   .def(
                       "__iter__", [](ListTag &self) { return py::make_iterator(self.begin(), self.end()); },
                       py::keep_alive<0, 1>())
                   .def("clear", &ListTag::clear, "Removes all tags from this list.")
                   .def(
                       "append", [](ListTag &self, const nbt::Tag &v) { self.emplace_back(v); }, py::arg("tag"), R"doc(
    Appends a tag to the end of this list.

    Args:
        tag: The tag to append. It must have the same type as the existing elements.
)doc")
                   .def(
                       "extend",
                       [](ListTag &self, py::iterable it) {
                           for (py::handle h : it) {
                               self.emplace_back(py::cast<nbt::Tag>(h));
                           }
                       },
                       py::arg("iterable"), R"doc(
    Appends all tags from an iterable to the end of this list.

    Args:
        iterable: An iterable of tags to append. Each tag must have the same type as the existing elements.
)doc")
                   .def(
                       "pop",
                       [](ListTag &self, py::ssize_t index) {
                           auto idx = normalize_index(index, self.size());
                           auto out = self.at(idx);  // copy out
                           self.erase(self.cbegin() + static_cast<std::ptrdiff_t>(idx));
                           return out;
                       },
                       py::arg("index") = -1, R"doc(
    Removes and returns the tag at the given index.

    Args:
        index: The index of the tag to remove. Defaults to the last tag.

    Returns:
        The removed tag.
)doc")
                   .def("to_list",
                        [](const ListTag &self) {
                            py::list lst;
                            for (const auto &elem : self) {
                                lst.append(tag_to_python(elem));
                            }
                            return lst;
                        },
                        "Converts this list to a plain Python list of native values.")
                   .def("size", &ListTag::size, "The number of tags in this list.")
                   .def("empty", &ListTag::empty, "Returns True if this list contains no tags.")
                   .def(py::self == py::self)
                   .def(py::self != py::self);
    bind_dump<ListTag>(cls);
}

static void bind_compound_tag(py::module &m)
{
    auto cls =
        py::class_<CompoundTag, nbt::TagBase>(m, "CompoundTag", "A named NBT compound tag mapping string keys to tags.")
            .def(py::init<>())
            .def(py::init([](py::dict d) {
                     CompoundTag c;
                     for (auto item : d) {
                         auto key = py::cast<std::string>(item.first);
                         auto val = py::cast<nbt::Tag>(item.second);
                         c.insert_or_assign(key, std::move(val));
                     }
                     return c;
                 }),
                 py::arg("mapping "))
            .def("__len__", &CompoundTag::size)
            .def("__bool__", [](const CompoundTag &self) { return !self.empty(); })
            .def("__contains__", &CompoundTag::contains, py::arg("key"))
            .def(
                "__getitem__", [](CompoundTag &self, const std::string &key) -> nbt::Tag & { return self.at(key); },
                py::return_value_policy::reference_internal)
            .def("__setitem__", [](CompoundTag &self, const std::string &key,
                                   const nbt::Tag &value) { self.insert_or_assign(key, value); })
            .def(
                "setdefault",
                [](CompoundTag &self, const std::string &key, const nbt::Tag &default_value) -> nbt::Tag & {
                    if (!self.contains(key)) {
                        self.insert_or_assign(key, default_value);
                    }
                    return self.at(key);
                },
                py::return_value_policy::reference_internal, py::arg("key"), py::arg("default"), R"doc(
    Returns the tag for the given key, inserting a default if the key is not present.

    Args:
        key: The key to look up.
        default: The tag to insert and return if the key is not present.

    Returns:
        The tag associated with the key, or the newly inserted default.
)doc")
            .def("clear", &CompoundTag::clear, "Removes all entries from this compound tag.")
            .def(
                "pop",
                [](CompoundTag &self, const std::string &key) {
                    if (self.contains(key)) {
                        auto removed = self.at(key);
                        self.erase(key);
                        return removed;
                    }
                    throw py::key_error("key not found: " + key);
                },
                py::arg("key"), R"doc(
    Removes the entry with the given key and returns its tag.

    Args:
        key: The key to remove.

    Returns:
        The tag that was associated with the key.

    Raises:
        KeyError: If the key is not present.
)doc")
            .def(
                "pop",
                [](CompoundTag &self, const std::string &key, py::object default_value) -> py::object {
                    if (self.contains(key)) {
                        auto removed = self.at(key);
                        self.erase(key);
                        return py::cast(removed);
                    }
                    return default_value;
                },
                py::arg("key"), py::arg("default") = py::none(), R"doc(
    Removes the entry with the given key and returns its tag, or a default if the key is not present.

    Args:
        key: The key to remove.
        default: The value to return if the key is not present.

    Returns:
        The tag that was associated with the key, or the default if the key is not present.
)doc")
            .def(
                "__iter__", [](CompoundTag &self) { return py::make_key_iterator(self.begin(), self.end()); },
                py::keep_alive<0, 1>())
            .def(
                "keys", [](CompoundTag &self) { return py::make_key_iterator(self.begin(), self.end()); },
                py::keep_alive<0, 1>(), "Returns an iterator over the keys in this compound tag.")
            .def(
                "values", [](CompoundTag &self) { return py::make_value_iterator(self.begin(), self.end()); },
                py::keep_alive<0, 1>(), "Returns an iterator over the tags in this compound tag.")
            .def(
                "items", [](CompoundTag &self) { return py::make_iterator(self.begin(), self.end()); },
                py::keep_alive<0, 1>(), "Returns an iterator over the (key, tag) pairs in this compound tag.")
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def("to_dict",
                 [](const CompoundTag &self) {
                     py::dict d;
                     for (const auto &[k, v] : self) {
                         d[py::str(k)] = tag_to_python(v);
                     }
                     return d;
                 },
                 "Converts this compound tag to a plain Python dict of native values.")
            .def("__str__", [](const CompoundTag &self) { return std::format("{}", self); })
            .def("__repr__", [](const CompoundTag &self) {
                py::dict d;
                for (const auto &[k, v] : self) {
                    d[py::str(k)] = py::cast(v);
                }
                return py::str("CompoundTag({})").format(py::repr(d));
            });
    bind_dump<CompoundTag>(cls);
}

void init_nbt(py::module_ &m)
{
    (void)py::class_<nbt::TagBase>(m, "Tag", "Base class for all NBT tag variants.");
    bind_value_tag<ByteTag>(m, "ByteTag", "An NBT tag holding an unsigned 8-bit integer value.");
    bind_value_tag<ShortTag>(m, "ShortTag", "An NBT tag holding a signed 16-bit integer value.");
    bind_value_tag<IntTag>(m, "IntTag", "An NBT tag holding a signed 32-bit integer value.");
    bind_value_tag<LongTag>(m, "LongTag", "An NBT tag holding a signed 64-bit integer value.");
    bind_value_tag<FloatTag>(m, "FloatTag", "An NBT tag holding a 32-bit floating-point value.");
    bind_value_tag<DoubleTag>(m, "DoubleTag", "An NBT tag holding a 64-bit floating-point value.");
    bind_array_tag<ByteArrayTag>(m, "ByteArrayTag", "An NBT tag holding an array of unsigned 8-bit integers.",
                                 py::buffer_protocol());
    bind_value_tag<StringTag>(m, "StringTag", "An NBT tag holding a string value.");
    bind_array_tag<IntArrayTag>(m, "IntArrayTag", "An NBT tag holding an array of signed 32-bit integers.");
    bind_list_tag(m);
    bind_compound_tag(m);

    m.def(
        "load",
        [](py::bytes data, const Literal<"little", "big"> &byte_order,
           bool network) -> std::tuple<endstone::nbt::Tag, py::str> {
            auto endian = byte_order.value() == "big" ? std::endian::big : std::endian::little;
            auto sv = std::string_view(PyBytes_AsString(data.ptr()), PyBytes_Size(data.ptr()));
            std::string name;
            auto tag = nbt::load(sv, name, endian, network);
            return std::make_tuple(std::move(tag), std::move(name));
        },
        py::arg("data"), py::arg("byte_order") = "little", py::arg("network") = false, R"doc(
    Deserialize binary NBT data into a tag.

    Args:
        data: Binary NBT data (bytes).
        byte_order: Byte order of the binary data, either ``'little'`` (Bedrock) or ``'big'`` (Java).
        network: If True, expect Bedrock network varint encoding.

    Returns:
        tuple[Tag, str]: A tuple of (tag, name) where name is the root tag name.
)doc");
}
}  // namespace endstone::python
