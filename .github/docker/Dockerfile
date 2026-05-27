# CI-only short path: installs a pre-built manylinux wheel into a slim
# runtime image. The from-source Dockerfile at the repo root is for
# local development.

FROM python:3.12-slim-bookworm

LABEL maintainer="Endstone <hello@endstone.dev>"

ENV PYTHONUNBUFFERED=1 \
    PYTHONIOENCODING=UTF-8

RUN apt-get update -y -qq \
    && apt-get install -y -qq curl gosu \
    && apt-get clean -y -qq \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash -u 1001 endstone

WORKDIR /home/endstone

COPY artifacts/*.whl ./
COPY docker-entrypoint.sh ./docker-entrypoint.sh

RUN pip install --no-cache-dir ./*.whl \
    && rm ./*.whl \
    && chmod +x ./docker-entrypoint.sh

EXPOSE 19132/udp 19133/udp

ENTRYPOINT ["./docker-entrypoint.sh"]
CMD ["endstone"]
