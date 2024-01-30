FROM python:3.12-slim-bullseye as base

LABEL maintainer="Endstone <hello@endstone.dev>"

ENV PYTHONUNBUFFERED=1 \
    PYTHONIOENCODING=UTF-8

FROM base AS builder

ARG LLVM_VERSION=15

RUN apt-get update -y -q \
    && apt-get install -y -q lsb-release wget software-properties-common gnupg \
    && wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh ${LLVM_VERSION} \
    && apt-get install -y -q libc++-${LLVM_VERSION}-dev libc++abi-${LLVM_VERSION}-dev \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld-${LLVM_VERSION} 100

ARG CMAKE_VERSION=3.26.6

ARG CMAKE_SH=cmake-${CMAKE_VERSION}-linux-x86_64.sh

RUN apt-get update -y -q \
    && apt-get install -y -q wget \
    && wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${CMAKE_SH} \
    && chmod +x ${CMAKE_SH} \
    && ./${CMAKE_SH} --skip-license --exclude-subdir --prefix=/usr/local

RUN apt-get update -y -q \
    && apt-get install -y -q git ninja-build

WORKDIR /usr/src/endstone

RUN git clone https://github.com/EndstoneMC/endstone.git .

RUN python -m pip install --upgrade pip \
    && pip install conan \
    && conan profile detect --force \
    && conan export third_party/funchook --version 1.1.3 \
    && conan install . --build=missing -pr:a .github/conan_profiles/linux

RUN python -m pip install --upgrade pip \
    && pip install wheel auditwheel setuptools "patchelf>=0.14" pytest \
    && python -m pip wheel . --no-deps --wheel-dir=wheelhouse --verbose \
    && python -m auditwheel --verbose repair --plat manylinux_2_31_x86_64 -w wheelhouse wheelhouse/*.whl \
    && pip install wheelhouse/*.whl \
    && pytest python/tests

FROM base AS final

RUN apt-get update -y -q \
    && apt-get install -y -q curl \
    && apt-get clean -y -q \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash endstone \
    && echo "endstone:endstone" | chpasswd \
    && adduser endstone sudo \
    && echo "endstone ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

WORKDIR /home/endstone

COPY --from=builder /usr/src/endstone/wheelhouse .

RUN python -m pip install --upgrade pip \
    && pip install *.whl \
    && rm *.whl

USER endstone

EXPOSE 19132/udp 19133/udp

CMD ["endstone"]
