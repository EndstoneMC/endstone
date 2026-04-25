---
comments: true
---

# Frequently Asked Questions

???+ question "Which platforms and architectures are supported by Endstone?"

    Endstone is built around the official [Bedrock Dedicated Server (BDS)](https://www.minecraft.net/download/server/bedrock), which supports Windows and Ubuntu Linux.

    Accordingly, Endstone only supports Windows and Ubuntu Linux. Other platforms may or may not function.

    BDS natively supports the `x86-64` CPU architecture. Systems with `arm` CPU architectures are not officially supported.

!!! tip "ARM Architecture (Apple Silicon, etc.)"
    While BDS is natively built for `x86-64`, it can run exceptionally well on `arm` systems using Docker emulation. In our testing on an **Apple M1 running macOS Tahoe (26.3.1)**, performance remains stable with little-to-no latency impact!