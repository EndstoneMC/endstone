---
comments: true
---

# Getting started

Endstone offers a powerful plugin API for Bedrock Dedicated Servers, the official server software for Minecraft: Bedrock
Edition. If you're familiar with Python, you can install Endstone with [`pip`](#with-pip), the Python package manager. If not,
we recommend using [`docker`](#with-docker).

## Installation

### Environment <small>optional</small> { #environment data-toc-label="Environment" }

We recommend using a [virtual environment], which is an isolated Python runtime.
If you are in a virtual environment, any packages that you install or upgrade
will be local to the environment. If you run into problems, you can
just delete and recreate the environment. It's trivial to set up:

-   Create a new virtual environment with:

    ```
    python3 -m venv .venv
    ```

-   Activate the environment with:

    === ":fontawesome-brands-windows: Command Prompt"

        ``` cmd
        . .venv\Scripts\activate.bat
        ```

    === ":fontawesome-brands-windows: PowerShell"

        ``` powershell
        . .venv\Scripts\Activate.ps1
        ```

    === ":fontawesome-brands-linux: Linux"

        ``` sh
        . .venv/bin/activate
        ```


    Your terminal should now print `(.venv)` before the prompt, which is how you
    know that you are inside the virtual environment that you just created.

-   Exit the environment with:

    ```
    deactivate
    ```

### with pip <small>recommended</small> { #with-pip data-toc-label="with pip" }

Endstone is published as a [Python package] and can be installed with `pip`, ideally by using the [virtual environment](#environment) 
from the last step. Open up a terminal and install Endstone with:

=== "Latest"

    ``` sh
    pip install endstone
    ```

### with docker

The official [Docker image] is a great way to get the Endstone server up and running in a few
minutes, and enables you to run Endstone on ARM through emulation. Open up a terminal and pull the image with:

=== "Latest"

    ```shell
    docker pull endstone/endstone
    ```

=== "Latest / :fontawesome-brands-apple: macOS / :fontawesome-solid-microchip: with emulation"

    ```shell
    docker pull --platform linux/amd64 endstone/endstone
    ```

    Note that if you are on an `x86-64` machine and you are not on macOS or Windows, emulation will not apply.

If you're new to Docker, check out the [Docker beginner's guide].

[Python package]: https://pypi.org/project/endstone/

[virtual environment]: https://realpython.com/what-is-pip/#using-pip-in-a-python-virtual-environment

[Docker image]: https://hub.docker.com/r/endstone/endstone/

[Docker beginner's guide]: https://docs.docker.com/get-started/