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

#ifdef ENDSTONE_WITH_SENTRY

#include "endstone/detail/sentry_handler.h"

#include <sentry.h>

namespace endstone::detail {

namespace {
sentry_value_t on_crash(const sentry_ucontext_t *ctx, sentry_value_t event, void *closure)
{
    // TODO(misc): print stack traces here
    return event;
}
}  // namespace

SentryHandler::SentryHandler()
{
    static_assert(sizeof(ENDSTONE_SENTRY_DSN) > 1, "The Sentry DSN is not set");
    sentry_options_t *options = sentry_options_new();
    sentry_options_set_dsn(options, ENDSTONE_SENTRY_DSN);
    sentry_options_set_database_path(options, "data/.sentry-native");
    // TODO: set crashpad handler path
    sentry_options_set_release(options, "endstone@" ENDSTONE_VERSION);
    sentry_options_set_environment(options, ENDSTONE_SENTRY_ENVIRONMENT);
    sentry_init(options);
}

SentryHandler::~SentryHandler()
{
    sentry_close();
}

}  // namespace endstone::detail

#endif
