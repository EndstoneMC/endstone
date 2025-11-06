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

#include "bedrock/world/attribute/mutable_attribute_with_context.h"

#include <cstddef>

#include "bedrock/world/attribute/attribute_instance.h"

float AttributeInstanceForwarder::getCurrentValue() const
{
    return instance->getCurrentValue();
}

void AttributeInstanceForwarder::setCurrentValue(float value)
{
    instance->setCurrentValue(value, context);
}

MutableAttributeWithContext::MutableAttributeWithContext(AttributeInstance *instance,
                                                         AttributeModificationContext context)
    : instance(instance), context(context)
{

}

bool MutableAttributeWithContext::isValid() const
{
    return instance != nullptr;
}

MutableAttributeWithContext::operator bool() const
{
    return isValid();
}

bool MutableAttributeWithContext::operator==(std::nullptr_t) const
{
    return instance == nullptr;
}

bool MutableAttributeWithContext::operator!=(std::nullptr_t) const
{
    return instance != nullptr;
}

AttributeInstanceForwarder *MutableAttributeWithContext::operator->()
{
    return reinterpret_cast<AttributeInstanceForwarder *>(this);
}

const AttributeInstanceForwarder *MutableAttributeWithContext::operator->() const
{
    return reinterpret_cast<const AttributeInstanceForwarder *>(this);
}
