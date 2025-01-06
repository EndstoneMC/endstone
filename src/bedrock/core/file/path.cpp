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

#include "bedrock/core/file/path.h"

namespace Core {

PathPart::PathPart(std::string &&str) : utf8_std_string_(std::move(str)) {}

PathPart::PathPart(std::string const &str) : utf8_std_string_(str) {}

PathPart::PathPart(gsl::not_null<const char *> c_str) : utf8_std_string_(c_str.get()) {}

PathPart::PathPart(gsl::not_null<const char *> c_str, std::size_t size) : utf8_std_string_(c_str.get(), size) {}

char const *PathPart::getUtf8CString() const
{
    return utf8_std_string_.c_str();
}

std::string const &PathPart::getUtf8StdString() const
{
    return utf8_std_string_;
}

std::size_t PathPart::size() const
{
    return utf8_std_string_.size();
}

bool PathPart::empty() const
{
    return utf8_std_string_.empty();
}

Path::Path(std::string &&str) : path_part_(str) {}

Path::Path(std::string const &str) : path_part_(str) {}

Path::Path(const char *c_str) : path_part_(gsl::not_null(c_str)) {}

Path::Path(const char *c_str, std::size_t size) : path_part_(gsl::not_null(c_str), size) {}

char const *Path::getUtf8CString() const
{
    return path_part_.getUtf8CString();
}

std::string_view Path::getUtf8StringView() const
{
    return path_part_.getUtf8StdString();
}

std::string const &Path::getUtf8StdString() const
{
    return path_part_.getUtf8StdString();
}

std::size_t Path::size() const
{
    return path_part_.size();
}

bool Path::empty() const
{
    return path_part_.empty();
}

Path const Path::EMPTY{};

}  // namespace Core
