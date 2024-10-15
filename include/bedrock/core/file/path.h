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

#include <string>

#include <gsl/gsl>

namespace Core {

class PathPart {
public:
    PathPart() = default;
    explicit PathPart(std::string &&str);
    explicit PathPart(std::string const &str);
    explicit PathPart(gsl::not_null<const char *> c_str);
    PathPart(gsl::not_null<const char *> c_str, std::size_t size);

    [[nodiscard]] char const *getUtf8CString() const;
    [[nodiscard]] std::string const &getUtf8StdString() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool empty() const;

private:
    std::string utf8_std_string_;
};

class Path {
public:
    Path() = default;
    explicit Path(std::string &&str);
    explicit Path(std::string const &str);
    explicit Path(const char *c_str);
    Path(const char *c_str, std::size_t size);

    [[nodiscard]] char const *getUtf8CString() const;
    [[nodiscard]] std::string_view getUtf8StringView() const;
    [[nodiscard]] std::string const &getUtf8StdString() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool empty() const;

    static Path const EMPTY;

private:
    PathPart path_part_;
};

template <typename T>
class PathBuffer {
public:
    PathBuffer() = default;
    explicit PathBuffer(T const &container) : container_(container) {}
    explicit PathBuffer(const char *c_str) : container_(c_str) {}
    explicit PathBuffer(Path const &path) : container_(path.getUtf8StdString()) {}

    T const &getContainer() const
    {
        return container_;
    };

private:
    T container_;
};

using HeapPathBuffer = PathBuffer<std::string>;

}  // namespace Core
