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

template <typename Tag, typename RawIdT = unsigned int, RawIdT RawInvalid = 0>
class TypedRuntimeId {
    RawIdT raw_id_;
};

using ContainerRuntimeId = TypedRuntimeId<struct ContainerRuntimeIdTag>;
static_assert(sizeof(ContainerRuntimeId) == 4);
