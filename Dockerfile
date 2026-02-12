FROM python:3.12-slim-bullseye AS base

LABEL maintainer="Endstone <hello@endstone.dev>"

ENV PYTHONUNBUFFERED=1 \
    PYTHONIOENCODING=UTF-8

# Build stage
FROM base AS builder

# Install required dependencies and configure LLVM
ARG LLVM_VERSION=20
RUN apt-get update -y -qq \
    && apt-get install -y -qq build-essential lsb-release git wget software-properties-common gnupg \
    && wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh ${LLVM_VERSION} \
    && apt-get install -y -qq libc++-${LLVM_VERSION}-dev libc++abi-${LLVM_VERSION}-dev clang-tools-${LLVM_VERSION} \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld-${LLVM_VERSION} 100

# Set default compiler and target platform tag for Python wheels
ENV CC=clang \
    CXX=clang++ \
    AUDITWHEEL_PLAT=manylinux_2_31_x86_64

# Define working directory for the source code
WORKDIR /usr/src/endstone

# Install C++ dependencies using Conan
COPY conanfile.py conanfile.py
RUN python -m pip install --upgrade pip \
    && pip install conan cmake ninja \
    && conan profile detect \
    && conan install . --build=missing -s compiler.cppstd=20 -s compiler.libcxx=libc++ -c tools.cmake.cmaketoolchain:generator=Ninja

# Copy the rest of the project files
COPY . .

# Build and test the project
RUN --mount=type=secret,id=sentry-auth-token,env=SENTRY_AUTH_TOKEN \
    pip install wheel auditwheel==6.2.0 sentry-cli setuptools "patchelf>=0.14" pytest \
    && python -m pip wheel . --no-deps --wheel-dir=dist --verbose \
    && python scripts/repair_wheel.py -o endstone -p endstone -w wheelhouse dist/*.whl \
    && pip install wheelhouse/*-${AUDITWHEEL_PLAT}.whl \
    && pytest tests/endstone/python

# Final stage
FROM python:3.12-slim-bookworm AS final

# Install runtime dependencies
RUN apt-get update -y -qq \
    && apt-get install -y -qq curl gosu \
    && apt-get clean -y -qq \
    && rm -rf /var/lib/apt/lists/*

# Create a non-root user for running the application
RUN useradd -m -s /bin/bash -u 1001 endstone

# Define working directory
WORKDIR /home/endstone

# Copy the built wheel files from the builder stage
COPY --from=builder /usr/src/endstone/wheelhouse .

# Install the wheel and clean up
RUN python -m pip install --no-cache-dir --upgrade pip \
    && pip install --no-cache-dir ./*.whl \
    && rm ./*.whl

# Copy entrypoint into workdir and make executable
COPY docker-entrypoint.sh ./docker-entrypoint.sh
RUN chmod +x ./docker-entrypoint.sh

# Expose application ports
EXPOSE 19132/udp 19133/udp

ENTRYPOINT ["./docker-entrypoint.sh"]
CMD ["endstone"]
