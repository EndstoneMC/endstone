---
comments: true
---

<!-- 
    Note that I've removed the language ID from links to official documentation.
    https://www.minecraft.net/en-us/download/server/bedrock -> https://www.minecraft.net/download/server/bedrock
    
    This is so someone navigating to those sites will see a page in their browser's language, if possible.
 -->

# Frequently Asked Questions

???+ question "Which platforms and architectures are supported by Endstone?"

    Endstone is built around the official [Bedrock Dedicated Server (BDS)], which supports Windows and Ubuntu Linux.

    Accordingly, Endstone only supports Windows and Ubuntu Linux. Endstone may or may not function on other platforms.

    BDS natively supports the `x86-64` CPU architecture. Systems with `arm` CPU architectures are not officially supported.[^1]

[^1]: While BDS is natively built for `x86-64`, it can also run well on `arm` systems via [Docker emulation]. Using emulation can also allow Endstone to run on macOS.

<!--
    Note that there is guidance on the capitalisation of "Add-On"
    https://learn.microsoft.com/en-us/minecraft/creator/documents/learningjourneyguide?view=minecraft-bedrock-stable#journey-1-learn-about-add-ons
    
    "Add-On" can refer to Marketplace Add-Ons as well as simpler add-ons that just involve Behavior and/or Resource Packs. Generally, we'll use capitalized Add-On for the former and lowercased add-on for the latter.

    I ignored that here, because it looks better (In my opinion) when "Add-On" is capitalised for this FAQ entry.
-->
???+ question "Does Endstone support Add-Ons?"

    Endstone is built around the official [Bedrock Dedicated Server (BDS)].

    Add-Ons, including behavior and resource packs, created for the base game will also work on Endstone.

    For more information, refer to [Microsoft's documentation](https://learn.microsoft.com/minecraft/creator/documents/bedrockserver/getting-started).


???+ question "Does Endstone support Data Packs?"

    Data packs are the Java Edition equivalent of behavior packs.

    Endstone, being a plugin API for Bedrock Dedicated Servers, does not support data packs, or any other content made for Java Edition which isn't already supported by BDS.

    For more information, see [Microsoft's documentation](https://learn.microsoft.com/minecraft/creator/documents/differencesbetweenbedrockandjava#behavior-packs).


<!-- This might end up being quite messy when the FAQ grows. Feel free to clean this up, anyone in the future! -->
<!-- Common Links -->
[Docker emulation]: installation.md/#with-docker-latest--macos--with-emulation
[Bedrock Dedicated Server (BDS)]: https://www.minecraft.net/download/server/bedrock
