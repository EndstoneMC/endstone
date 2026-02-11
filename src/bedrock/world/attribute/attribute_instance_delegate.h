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

#include "bedrock/forward.h"
#include "bedrock/world/attribute/attribute_instance_handle.h"

class AttributeInstanceDelegate {
public:
    AttributeInstanceDelegate(const AttributeInstance &);
    virtual ~AttributeInstanceDelegate() = default;
    virtual void tick(AttributeInstance &, AttributeModificationContext &) = 0;
    virtual void notify(AttributeMessageType, AttributeModificationContext &) = 0;
    virtual bool willChange(float, float, const AttributeBuff &) = 0;
    virtual std::optional<float> change(float, float, const AttributeBuff &) = 0;
    [[nodiscard]] virtual float getBuffValueWithModifiers(const AttributeBuff &) const = 0;

protected:
    const AttributeInstance &_getInstance(AttributeModificationContext &) const;
    AttributeInstance *_getMutableInstance(AttributeModificationContext &) const;

private:
    AttributeInstanceHandle attribute_handle_;
};
static_assert(sizeof(AttributeInstanceDelegate) == 16);
