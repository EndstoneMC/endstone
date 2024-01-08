FROM ubuntu:22.04 as base
LABEL maintainer="Endstone <hello@endstone.dev>"
ENV PYTHONUNBUFFERED 1
ENV PYTHONIOENCODING UTF-8
ARG DEBIAN_FRONTEND=noninteractive

FROM base AS python
ENV PYENV_ROOT="/usr/src/.pyenv"
ENV PATH="$PYENV_ROOT/shims:$PYENV_ROOT/bin:$PATH"
RUN apt-get update -yq && \
    apt-get install -yq build-essential git curl libsqlite3-dev libssl-dev zlib1g-dev libbz2-dev libreadline-dev libncursesw5-dev xz-utils tk-dev libxml2-dev libxmlsec1-dev libffi-dev liblzma-dev && \
    curl https://pyenv.run | bash && \
    pyenv install 3.11 && \
    pyenv global 3.11 && \
    apt-get purge -yq build-essential git curl libsqlite3-dev libssl-dev zlib1g-dev libbz2-dev libreadline-dev libncursesw5-dev xz-utils tk-dev libxml2-dev libxmlsec1-dev libffi-dev liblzma-dev && \
    apt-get autoremove -yq && \
    apt-get clean -yq && \
    rm -rf /var/lib/apt/lists/*

FROM python AS build
WORKDIR /usr/src/endstone
COPY . .
RUN apt-get update -yq && \
    apt-get install -yq build-essential git curl libsqlite3-dev && \
    python3 -m pip install --upgrade pip && \
    pip3 install conan cmake ninja && \
    conan profile detect --force && \
    conan export third_party/pybind11 && \
    conan export third_party/funchook --version 1.1.3 && \
    conan install . --build=missing -s compiler.cppstd=17 -c tools.cmake.cmaketoolchain:generator=Ninja && \
    pip3 install --no-cache . && \
    apt-get purge -yq build-essential git && \
    apt-get autoremove -yq && \
    apt-get clean -yq && \
    rm -rf /var/lib/apt/lists/*

FROM build as final
CMD ["/bin/bash"]