# syntax=docker/dockerfile:1

ARG PYTHON_VERSION=3.12

# ──────────────────────────────────────────────────────────────────────────────
# Build stage
#
# Bullseye is intentional here: its glibc 2.31 is what lets auditwheel tag the
# wheels as manylinux_2_31 (see AUDITWHEEL_PLAT). The runtime stage below uses a
# newer Debian release — do not "align" the two without checking the wheel tag.
# ──────────────────────────────────────────────────────────────────────────────
FROM python:${PYTHON_VERSION}-slim-bullseye AS builder

# Install the LLVM toolchain and build dependencies.
ARG LLVM_VERSION=20
RUN apt-get update -y -qq \
    && apt-get install -y -qq --no-install-recommends \
        autoconf automake build-essential ca-certificates git gnupg libtool m4 \
        lsb-release software-properties-common wget \
    && wget -q https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh ${LLVM_VERSION} \
    && apt-get install -y -qq --no-install-recommends \
        libc++-${LLVM_VERSION}-dev libc++abi-${LLVM_VERSION}-dev clang-tools-${LLVM_VERSION} \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-${LLVM_VERSION} 100 \
    && update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld-${LLVM_VERSION} 100 \
    && rm -rf /var/lib/apt/lists/* llvm.sh

# Set default compiler and target platform tag for Python wheels.
ENV CC=clang \
    CXX=clang++ \
    AUDITWHEEL_PLAT=manylinux_2_31_x86_64 \
    PIP_DISABLE_PIP_VERSION_CHECK=1

# Define working directory for the source code.
WORKDIR /usr/src/endstone

# Install C++ dependencies with Conan first so this layer is cached across
# source-only changes.
COPY conanfile.py conanfile.py
RUN python -m pip install --upgrade pip \
    && pip install conan cmake ninja \
    && conan profile detect \
    && conan install . --build=missing --build=m4/* -s compiler.cppstd=20 -s compiler.libcxx=libc++ -c tools.cmake.cmaketoolchain:generator=Ninja

# Copy the rest of the project files.
COPY . .

# Build, repair and test the project.
RUN --mount=type=secret,id=sentry-auth-token,env=SENTRY_AUTH_TOKEN \
    pip install wheel auditwheel==6.2.0 sentry-cli setuptools "patchelf>=0.14" pytest \
    && python -m pip wheel . --no-deps --wheel-dir=dist --verbose \
    && python scripts/repair_wheel.py -o endstone -p endstone -w wheelhouse dist/*.whl \
    && pip install wheelhouse/*-${AUDITWHEEL_PLAT}.whl \
    && pytest tests

# ──────────────────────────────────────────────────────────────────────────────
# Runtime stage
# ──────────────────────────────────────────────────────────────────────────────
FROM python:${PYTHON_VERSION}-slim-bookworm AS final

LABEL org.opencontainers.image.title="Endstone" \
      org.opencontainers.image.description="A plugin API for Bedrock Dedicated Servers, supporting both Python and C++." \
      org.opencontainers.image.source="https://github.com/EndstoneMC/endstone" \
      org.opencontainers.image.documentation="https://endstone.dev" \
      org.opencontainers.image.licenses="Apache-2.0" \
      org.opencontainers.image.vendor="EndstoneMC" \
      maintainer="Endstone <hello@endstone.dev>"

ENV PYTHONUNBUFFERED=1 \
    PYTHONIOENCODING=UTF-8 \
    PIP_NO_CACHE_DIR=1 \
    PIP_DISABLE_PIP_VERSION_CHECK=1 \
    PUID=1000 \
    PGID=1000 \
    TZ=UTC

# Install runtime dependencies and create the unprivileged service user. The
# entrypoint remaps this user to PUID/PGID at startup, then drops to it via gosu.
RUN apt-get update -y -qq \
    && apt-get install -y -qq --no-install-recommends gosu tzdata \
    && apt-get clean -y -qq \
    && rm -rf /var/lib/apt/lists/* \
    && groupadd --gid 1000 endstone \
    && useradd --uid 1000 --gid 1000 --create-home --shell /bin/bash endstone \
    && mkdir -p /data \
    && chown endstone:endstone /data

# Install the repaired wheel built in the previous stage.
COPY --from=builder /usr/src/endstone/wheelhouse /tmp/wheelhouse
RUN pip install /tmp/wheelhouse/*.whl \
    && rm -rf /tmp/wheelhouse

# Install the container scripts outside /data so a bind mount cannot hide them.
COPY docker/entrypoint.sh /usr/local/bin/entrypoint.sh
COPY docker/healthcheck.py /usr/local/bin/healthcheck.py
RUN chmod +x /usr/local/bin/entrypoint.sh

# /data holds the world, plugins and configuration — mount it to persist them.
WORKDIR /data
VOLUME ["/data"]

# Expose the Bedrock server ports (IPv4 and IPv6).
EXPOSE 19132/udp 19133/udp

# Report liveness by pinging the server over RakNet.
HEALTHCHECK --interval=30s --timeout=5s --start-period=120s --retries=5 \
    CMD ["python", "/usr/local/bin/healthcheck.py"]

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
CMD ["endstone", "--server-folder", "/data", "--yes"]
