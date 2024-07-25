

# File message\_form.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**form**](dir_0fd3b458603af3963ebb9c312a9238ec.md) **>** [**message\_form.h**](message__form_8h.md)

[Go to the documentation of this file](message__form_8h.md)


```C++
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

#include <utility>

#include "endstone/form/form.h"

namespace endstone {

class MessageForm : public Form<MessageForm> {
public:
    using OnSubmitCallback = std::function<void(Player *, int)>;

    [[nodiscard]] Message getContent() const
    {
        return content_;
    }

    MessageForm &setContent(Message text)
    {
        content_ = std::move(text);
        return *this;
    }

    [[nodiscard]] Message getButton1() const
    {
        return button1_text_;
    }

    MessageForm &setButton1(Message text)
    {
        button1_text_ = std::move(text);
        return *this;
    }

    [[nodiscard]] Message getButton2() const
    {
        return button2_text_;
    }

    MessageForm &setButton2(Message text)
    {
        button2_text_ = std::move(text);
        return *this;
    }

    [[nodiscard]] OnSubmitCallback getOnSubmit() const
    {
        return on_submit_;
    }

    MessageForm &setOnSubmit(OnSubmitCallback on_submit)
    {
        on_submit_ = std::move(on_submit);
        return *this;
    }

private:
    Message content_;
    Message button1_text_;
    Message button2_text_;
    OnSubmitCallback on_submit_;
};

}  // namespace endstone
```


