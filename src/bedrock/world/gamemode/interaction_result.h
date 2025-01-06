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

class InteractionResult {

    enum Result : unsigned int {
        FAILURE = 0,
        SUCCESS = 1,
        SWING = 2,
    };

public:
    InteractionResult() = default;
    explicit InteractionResult(std::underlying_type_t<Result> result) : result_(result) {}

    [[nodiscard]] bool shouldSwing() const
    {
        return result_ == SWING;
    }

    [[nodiscard]] bool isSuccessful() const
    {
        return result_ == SUCCESS || result_ == SWING;
    }

    operator bool() const
    {
        return isSuccessful();
    }

    static InteractionResult Success()
    {
        static InteractionResult result{SWING};
        return result;
    }

    static InteractionResult SuccessNoSwing()
    {
        static InteractionResult result{SUCCESS};
        return result;
    }

    static InteractionResult Failure()
    {
        static InteractionResult result{FAILURE};
        return result;
    }

private:
    std::underlying_type_t<Result> result_;
};
