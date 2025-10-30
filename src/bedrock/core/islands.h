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

namespace Bedrock {
class ActivationArguments;
class SignalReceiver;

using IslandId = uint16_t;

class IIslandCore {
public:
    virtual ~IIslandCore() = default;
    virtual IslandId getId() = 0;
    virtual bool start() = 0;
    virtual bool suspend() = 0;
    virtual bool resume() = 0;
    virtual bool stop() = 0;
    virtual void mainUpdate() = 0;
    virtual void processActivationArguments(const ActivationArguments &) = 0;
};

class AppIsland : public IIslandCore {
public:
    AppIsland();
    ~AppIsland() override = default;
    IslandId getId() override;
    virtual bool start();
    virtual bool suspend();
    virtual bool resume();
    virtual bool stop();
    virtual void mainUpdate();
    virtual void processActivationArguments(const ActivationArguments &);
    std::unique_ptr<SignalReceiver> signal_rcvr;
};
}  // namespace Bedrock
