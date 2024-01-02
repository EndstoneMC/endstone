FROM ubuntu:22.04 AS base

LABEL maintainer="Endstone <hello@endstone.dev>"

ENV LC_ALL="en_US.UTF-8" \
    LANG="en_US.UTF-8" \
    LANGUAGE="en_US.UTF-8"\
    DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:deadsnakes/ppa

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        curl \
        git \
        python3.11 \
        python3.11-dev \
        python3-pip \
        && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

FROM base AS build

WORKDIR /usr/src/endstone

COPY . /usr/src/endstone

RUN python3.11 -m pip install --upgrade pip && \
    pip3 install conan>=2.0 cmake ninja

RUN conan profile detect --force && \
    conan export third_party/pybind11 && \
    conan install . --build=missing -s compiler.cppstd=17 -c tools.cmake.cmaketoolchain:generator=Ninja

RUN pip3 install --no-cache .

FROM build as final

CMD ["/bin/bash"]