# syntax=docker/dockerfile:1
# CI release path that installs the pre-built manylinux wheel instead of building from source.

ARG PYTHON_VERSION=3.13
FROM python:${PYTHON_VERSION}-slim-bookworm

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

# Install the pre-built wheel staged by the build.yml docker job.
COPY artifacts/*.whl /tmp/wheelhouse/
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
