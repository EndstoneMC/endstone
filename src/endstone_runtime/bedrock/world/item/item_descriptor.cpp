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

#include "bedrock/world/item/item_descriptor.h"

void ItemDescriptor::serialize(Json::Value &json) const
{
    if (impl_) {
        impl_->serialize(json);
    }
}

void ItemDescriptor::serialize(BinaryStream &stream) const
{
    if (impl_ && impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    auto type = impl_ ? impl_->getType() : InternalType::Invalid;
    stream.writeByte(static_cast<std::uint8_t>(type));
    if (impl_) {
        impl_->serialize(stream);
    }
}

const Item *ItemDescriptor::getItem() const
{
    if (!impl_) {
        return nullptr;
    }
    if (impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    return impl_->getItem().item;
}

std::int16_t ItemDescriptor::getAuxValue() const
{
    if (!impl_) {
        return ANY_AUX_VALUE;
    }
    if (impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    return impl_->getItem().aux_value;
}

std::string const &ItemDescriptor::getFullName() const
{
    static std::string empty;
    if (!impl_) {
        return empty;
    }
    return impl_->getFullName();
}

struct ItemTagDescriptor : ItemDescriptor::BaseDescriptor {
    ItemTagDescriptor(const ItemTag &item_tag) : item_tag_(item_tag){};

private:
    ItemTag item_tag_;
};
