# syntax=docker/dockerfile:1

ARG PYTHON_VERSION=3.13
ARG PYTHON_TAG=cp313

FROM scratch AS conan-cache

# ──────────────────────────────────────────────────────────────────────────────
# Build stage
#
# The same image the published wheels are built in: it carries the LLVM
# toolchain, libc++ and the glibc 2.31 floor that tags the wheels manylinux_2_31.
# ──────────────────────────────────────────────────────────────────────────────
FROM ghcr.io/endstonemc/manylinux_2_31_x86_64_clang20 AS builder

ARG PYTHON_TAG
ENV PATH=/opt/python/${PYTHON_TAG}-${PYTHON_TAG}/bin:${PATH} \
    PIP_DISABLE_PIP_VERSION_CHECK=1

# Define working directory for the source code.
WORKDIR /usr/src/endstone

# install C++ deps first so this layer caches across source-only changes
COPY .conanrc .conanrc
COPY .conan2/remotes.json .conan2/remotes.json
COPY .conan2/profiles/default .conan2/profiles/default
COPY conanfile.py conanfile.py
RUN --mount=type=bind,from=conan-cache,target=/tmp/conan-cache \
    python -m pip install --upgrade pip \
    && pip install conan ninja \
    && if [ -d /tmp/conan-cache/p ]; then cp -a /tmp/conan-cache/p .conan2/p; fi \
    && conan install . --build=missing

# Copy the rest of the project files.
COPY . .

# Build, repair and test the project.
RUN --mount=type=secret,id=sentry-auth-token,env=SENTRY_AUTH_TOKEN \
    pip install wheel setuptools pytest uv \
    && python -m pip wheel . --no-deps --wheel-dir=dist --verbose \
    && uv run --script scripts/repair_wheel.py -o endstone -p endstone -w wheelhouse dist/*.whl \
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

# runtime deps and the unprivileged service user
# libcurl4 provides libcurl.so.4 for the Bedrock server binary
RUN apt-get update -y -qq \
    && apt-get install -y -qq --no-install-recommends gosu tzdata libcurl4 \
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