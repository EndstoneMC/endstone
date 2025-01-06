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

#if defined(_WIN32)
#define closesocket__ closesocket
#define select__      select
#endif
#define accept__      accept
#define connect__     connect
#define socket__      socket
#define bind__        bind
#define getsockname__ getsockname
#define getsockopt__  getsockopt
#define inet_addr__   inet_addr
#define ioctlsocket__ ioctlsocket
#define listen__      listen
#define recv__        recv
#define recvfrom__    recvfrom
#define sendto__      sendto
#define send__        send
#define setsockopt__  setsockopt
#define shutdown__    shutdown
#define WSASendTo__   WSASendTo
