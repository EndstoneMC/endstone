---
name: build-image
description: Builds the Endstone Docker image locally from the root Dockerfile or the release Dockerfile and smoke-tests the container's entrypoint, volume ownership, healthcheck and shutdown. Use when asked to "build the docker image", "test the Dockerfile" or "try docker compose", or after a change to Dockerfile, .github/docker, docker/ or docker-compose.yml.
---

# Build Image

Read first: `.claude/rules/dev/docker.md`, `.claude/rules/build.md`.

1. Confirm `docker info` succeeds.
2. From-source image: run `docker build -t endstone:local .` from the repo root and wait for the in-build `pytest tests` to pass.
3. Release image: copy a cp313 manylinux wheel into `artifacts/`, run `docker build -f .github/docker/Dockerfile -t endstone:local-release .`, then delete `artifacts/`.
4. Start a container on a named volume: `docker run -d --name endstone-smoke -p 20262:19132/udp -v endstone-smoke:/data endstone:local`.
5. Follow `docker logs -f endstone-smoke` until it shows `Server started.`
6. Poll `docker inspect --format '{{.State.Health.Status}}' endstone-smoke` until it reads `healthy`.
7. Run `docker exec endstone-smoke id` and `docker exec endstone-smoke ls -ln /data` and check both show uid and gid 1000.
8. Run `docker stop endstone-smoke` and check it exits 0 within the grace period.
9. Run `docker rm endstone-smoke` and `docker volume rm endstone-smoke`.
10. For a Compose change, run `docker compose up -d`, repeat steps 5-8 against its container, then `docker compose down`.
11. Report each image's build result, health status and ownership check.
