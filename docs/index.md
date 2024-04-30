![endstone](https://socialify.git.ci/endstonemc/endstone/image?description=1&font=Jost&forks=1&issues=1&logo=https%3A%2F%2Fstatic.wikia.nocookie.net%2Fminecraft_gamepedia%2Fimages%2F4%2F43%2FEnd_Stone_JE3_BE2.png&name=1&owner=1&pulls=1&stargazers=1&theme=Light)

<!-- Synchronize Socialify URL theme with palette -->
<script>
    var imageUrl = document.querySelector("img[src*='socialify']");

    var palette = __md_get("__palette");
    if (palette && typeof palette.color === "object") {
        var theme = palette.color.scheme === "slate"
            ? "Dark"
            : "Light";

        imageUrl.src = imageUrl.src.replace(/&theme=.*$/, '&theme=' + theme);
    }

    document.addEventListener("DOMContentLoaded", function () {
        var ref = document.querySelector("[data-md-component=palette]");
        ref.addEventListener("change", function () {
            var palette = __md_get("__palette");
            if (palette && typeof palette.color === "object") {
                var theme = palette.color.scheme === "slate"
                    ? "Dark"
                    : "Light";

                imageUrl.src = imageUrl.src.replace(/&theme=.*$/, '&theme=' + theme);
            }
        })
    })
</script>

# 👋 Welcome to Endstone  
Endstone offers a powerful plugin API for Bedrock Dedicated Servers, supporting both Python and C++ languages. This
empowers developers with the tools to modify and enhance the Bedrock Dedicated Servers with custom features and
functionality, letting them design innovative and bespoke player experiences.

## 🚀 Ready to Start?

<div class="grid cards" markdown>

-   **Install Endstone server**
  
    ---
  
    Follow the comprehensive guide to install and configure your Endstone server.
  
    [:octicons-arrow-right-24: Getting Started](getting-started.md)
  
-   **Write your first plugin**
  
    ---
  
    Our step-by-step tutorial takes you from zero to hero as you create your first plugin.
  
    [:octicons-arrow-right-24: Tutorials](tutorials/create-your-first-plugin.md)

</div>