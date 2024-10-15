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

#include "bedrock/core/resource/content_identity.h"
#include "bedrock/core/resource/resource_helper.h"
#include "bedrock/core/utility/enable_non_owner_references.h"

class IContentKeyProvider : public Bedrock::EnableNonOwnerReferences {
public:
    [[nodiscard]] virtual std::string getContentKey(ContentIdentity const &) const = 0;
    [[nodiscard]] virtual std::string getAlternateContentKey(ContentIdentity const &) const;
    [[nodiscard]] virtual bool requireEncryptedReads() const;
    virtual void setTempContentKeys(ContentKeyMap const &) = 0;
    virtual void clearTempContentKeys() = 0;
};

class IContentAccessibilityProvider : public IContentKeyProvider {
public:
    [[nodiscard]] virtual bool canAccess(ContentIdentity const &) const = 0;
};
