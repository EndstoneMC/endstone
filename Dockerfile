FROM ubuntu:22.04 as base

LABEL maintainer="Endstone <hello@endstone.dev>"

ENV PYTHONUNBUFFERED 1
ENV PYTHONIOENCODING UTF-8

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y build-essential git curl libssl-dev zlib1g-dev libbz2-dev libreadline-dev libsqlite3-dev libncursesw5-dev xz-utils tk-dev libxml2-dev libxmlsec1-dev libffi-dev liblzma-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

ENV PYENV_ROOT="/usr/src/.pyenv"
ENV PATH="$PYENV_ROOT/shims:$PYENV_ROOT/bin:$PATH"

# Install pyenv
RUN curl https://pyenv.run | bash

# Install required Python versions
RUN pyenv install 3.11
RUN pyenv global 3.11

FROM base AS build

WORKDIR /usr/src/endstone
COPY . .

RUN python3 -m pip install --upgrade pip && \
    pip3 install conan>=2.0 cmake ninja

RUN conan profile detect --force && \
    conan export third_party/pybind11 && \
    conan install . --build=missing -s compiler.cppstd=17 -c tools.cmake.cmaketoolchain:generator=Ninja

RUN pip3 install --no-cache .

FROM build as final

CMD ["/bin/bash"]
