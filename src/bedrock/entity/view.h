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

#include "bedrock/entity/entity_id.h"

template <typename Component>
class Optional {
public:
    using storage_type = entt::storage_for_t<Component, EntityId>;
    using value_type = Component;

    Optional(storage_type &storage, EntityId entity) : entt_storage_(&storage), entity_(entity) {}
    ~Optional() {}  // NOLINT(*-use-equals-default): keep it as-is for ABI compatibility, do not simplify it to default
    explicit operator bool() const { return entt_storage_->contains(entity_); }
    value_type &operator*() const { return entt_storage_->get(entity_); }

private:
    storage_type *entt_storage_;
    EntityId entity_;
};

namespace details {
template <typename...>
struct EntityNatvisHelper {
    EntityId entity;
};

template <typename T>
struct EntityViewer {};

template <typename... Components>
struct filter_to_type_lists;

template <typename T>
struct filter_to_type_lists<T> {
    using returned_type_list = entt::type_list<T>;
};

template <typename T>
struct filter_to_type_lists<Optional<T>> {
    using returned_type_list = entt::type_list<>;
};

template <typename T, typename... Rest>
struct filter_to_type_lists<Optional<T>, Rest...> {
    using returned_type_list = filter_to_type_lists<Rest...>::returned_type_list;
};

template <typename T, typename... Rest>
struct filter_to_type_lists<T, Rest...> {
    using returned_type_list =
        entt::type_list_cat_t<entt::type_list<T>, typename filter_to_type_lists<Rest...>::returned_type_list>;
};

// Endstone begins
template <class TypeList>
struct view_type_from_list;

template <class... Ts>
struct view_type_from_list<entt::type_list<Ts...>> {
    using type = decltype(std::declval<EnTTRegistry &>().template view<Ts...>());
};
// Endstone ends
}  // namespace details

template <typename EntityContextT, typename... Components>
class ViewT {
    using returned_type_list = details::filter_to_type_lists<Components...>::returned_type_list;
    using ViewType = details::view_type_from_list<returned_type_list>::type;

public:
    ViewT(EntityRegistry &registry) : registry_{registry} {}
    auto tryGetAll(const EntityContextT &context) const
        -> std::optional<decltype(std::declval<ViewType &>().get(context._getEntityId()))>
    {
        auto &view = _get();
        if (view.contains(context._getEntityId())) {
            return view.get(context._getEntityId());
        }
        return std::nullopt;
    }

private:
    ViewType &_get() const { return view_; }
    EntityRegistry &registry_;
    mutable ViewType view_;
    details::EntityViewer<ViewType> entity_viewer_;
};
