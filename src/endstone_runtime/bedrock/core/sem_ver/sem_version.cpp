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

#include "bedrock/core/sem_ver/sem_version.h"

#include <utility>

#include <fmt/format.h>

SemVersion::SemVersion() : full_version_string_("0.0.0") {}

SemVersion::SemVersion(any_version_constructor) : full_version_string_("*"), valid_version_(true), any_version_(true) {}

SemVersion::SemVersion(std::uint16_t major, std::uint16_t minor, std::uint16_t patch, std::string pre_release,
                       std::string build_meta)
    : major_(major), minor_(minor), patch_(patch), pre_release_(std::move(pre_release)),
      build_meta_(std::move(build_meta)), valid_version_(true)
{
    full_version_string_ = fmt::format("{}.{}.{}", major_, minor_, patch_);
    if (!pre_release_.empty()) {
        full_version_string_ += pre_release_;
    }
    if (!build_meta_.empty()) {
        full_version_string_ += build_meta_;
    }
}

const std::string &SemVersion::asString() const
{
    return full_version_string_;
}

bool SemVersion::operator==(const SemVersion &rhs) const
{
    if (this->any_version_ && rhs.any_version_) {
        return true;
    }

    if (this->any_version_ || rhs.any_version_) {
        return false;
    }

    if (this->major_ != rhs.major_ || this->minor_ != rhs.minor_ || this->patch_ != rhs.patch_) {
        return false;
    }

    if (this->pre_release_ != rhs.pre_release_) {
        return false;
    }
    return true;
}
