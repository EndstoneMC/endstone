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

#include "bedrock/resources/base_game_version.h"

BaseGameVersion::BaseGameVersion() = default;

BaseGameVersion::BaseGameVersion(bool never_compatible) : never_compatible_(never_compatible) {}

BaseGameVersion::BaseGameVersion(any_version_constructor) : sem_version_(SemVersion::AnyVersionConstructor) {}

bool BaseGameVersion::operator==(const BaseGameVersion &rhs) const
{
    if (rhs.never_compatible_) {
        if (never_compatible_) {
            return true;
        }
    }
    else if (never_compatible_) {
        return false;
    }
    return sem_version_ == rhs.sem_version_;
}

bool BaseGameVersion::isCompatibleWith(const BaseGameVersion &rhs) const
{
    return !never_compatible_ && !rhs.never_compatible_ &&
           (!sem_version_.isValid() || rhs.sem_version_.isAnyVersion() || sem_version_ <= rhs.sem_version_);
}

bool BaseGameVersion::isNeverCompatible() const
{
    return never_compatible_;
}
