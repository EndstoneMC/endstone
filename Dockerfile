# syntax=docker/dockerfile:1

FROM python:3.12-slim-bullseye AS base

LABEL maintainer="Endstone <hello@endstone.dev>"

ENV PYTHONUNBUFFERED=1 \
    PYTHONIOENCODING=UTF-8 \
    AUDITWHEEL_PLAT=manylinux_2_31_x86_64

FROM base AS builder

ARG LLVM_VERSION=16

RUN apt-get update -y -qq \
    && apt-get install -y -qq build-essential lsb-release wget software-properties-common gnupg \
    && wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh ${LLVM_VERSION} \
    && apt-get install -y -qq libc++-${LLVM_VERSION}-dev libc++abi-${LLVM_VERSION}-dev \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld-${LLVM_VERSION} 100

ENV CC=clang \
    CXX=clang++

ARG CMAKE_VERSION=3.31.4

ARG CMAKE_SH=cmake-${CMAKE_VERSION}-linux-x86_64.sh

RUN apt-get update -y -qq \
    && apt-get install -y -qq wget \
    && wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${CMAKE_SH} \
    && chmod +x ${CMAKE_SH} \
    && ./${CMAKE_SH} --skip-license --exclude-subdir --prefix=/usr/local

RUN apt-get update -y -qq \
    && apt-get install -y -qq git ninja-build

WORKDIR /usr/src/endstone

COPY . .

RUN --mount=type=cache,target=/root/.conan2/p \
    --mount=type=secret,id=sentry-auth-token,env=SENTRY_AUTH_TOKEN \
    python -m pip install --upgrade pip \
    && pip install wheel auditwheel sentry-cli setuptools "patchelf>=0.14" pytest \
    && python -m pip wheel . --no-deps --wheel-dir=dist --verbose \
    && python scripts/repair_wheel.py -o endstone -p endstone -w wheelhouse dist/*.whl \
    && pip install wheelhouse/*-${AUDITWHEEL_PLAT}.whl \
    && pytest tests/endstone/python

FROM base AS final

RUN apt-get update -y -qq \
    && apt-get install -y -qq curl \
    && apt-get clean -y -qq \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash endstone \
    && printf "endstone:endstone" | chpasswd \
    && adduser endstone sudo \
    && printf "endstone ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

WORKDIR /home/endstone

COPY --from=builder /usr/src/endstone/wheelhouse .

RUN python -m pip install --no-cache-dir --upgrade pip \
    && pip install --no-cache-dir ./*-${AUDITWHEEL_PLAT}.whl \
    && rm ./*.whl

USER endstone

EXPOSE 19132/udp 19133/udp

CMD ["endstone"]
