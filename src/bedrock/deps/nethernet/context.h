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

#include <cstdarg>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/core/utility/non_owner_pointer.h"

namespace NetherNet {

class ILanDiscovery;
class RtcThreadManager;
struct GlobalConfiguration;
enum class LogSeverity : int;

class IContext : public Bedrock::EnableNonOwnerReferences {
public:
    ~IContext() override;
    [[nodiscard]] virtual const RtcThreadManager &getSignalThread() const = 0;
    [[nodiscard]] virtual const RtcThreadManager &getNetworkThread() const = 0;
    [[nodiscard]] virtual const RtcThreadManager &getWorkerThread() const = 0;
    virtual ILanDiscovery &getLanDiscovery() = 0;
    virtual void onSuspend() = 0;
    virtual void onResume() = 0;
    [[nodiscard]] virtual const GlobalConfiguration &getGlobalConfig() const = 0;
    virtual void _logMessage(LogSeverity, const char *, va_list) const = 0;
    virtual void setLoggingLevel(LogSeverity) = 0;
};

class ContextProxy : IContext {
public:
    ~ContextProxy() override;

private:
    Bedrock::NotNullNonOwnerPtr<IContext> target_;
};
BEDROCK_STATIC_ASSERT_SIZE(ContextProxy, 48, 48);

}  // namespace NetherNet
