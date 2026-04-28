---
comments: true
---

# Frequently Asked Questions

???+ question "Which platforms and architectures are supported by Endstone?"

    Endstone is built around the official [Bedrock Dedicated Server (BDS)](https://www.minecraft.net/download/server/bedrock), which supports Windows and Ubuntu Linux.

    Accordingly, Endstone only supports Windows and Ubuntu Linux. Other platforms may or may not function.

    BDS natively supports the `x86-64` CPU architecture. Systems with `arm` CPU architectures are not officially supported.[^1]

[^1]: While BDS is natively built for `x86-64`, it can also run well on `arm` systems via [Docker emulation]. Using emulation can also allow Endstone to run on macOS.

[Docker emulation]: installation.md/#with-docker-latest-arm-with-emulation