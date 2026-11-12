---
paths:
  - "Dockerfile"
  - ".github/docker/**"
  - "docker/**"
  - "docker-compose.yml"
  - ".dockerignore"
  - ".github/workflows/docker.yml"
---

# Docker

## Dockerfiles

- The root `Dockerfile` is the from-source build that `docker.yml` publishes as `ghcr.io/endstonemc/endstone:nightly`.
- `.github/docker/Dockerfile` is the release image, built by the `docker` job in `build.yml` from the cp313 manylinux wheel copied to `artifacts/*.whl`.
- Keep the runtime stage of both Dockerfiles identical: base, labels, env, user, scripts, volume, ports, healthcheck, entrypoint and cmd.
- Build the root builder stage on `ghcr.io/endstonemc/manylinux_2_31_x86_64_clang20`, the image `[tool.cibuildwheel]` uses.
- The builder stage runs `pip wheel`, `scripts/repair_wheel.py`, an install of the repaired wheel, then `pytest tests`; a failing test fails the image build.
- Use `python:<version>-slim-bookworm` as the runtime base with the `PYTHON_VERSION` build arg, default 3.13, and keep `PYTHON_TAG` equal to it in `cpNNN` form.
- Change the release image's base Python and the wheel glob in `build.yml` together.
- Build for `linux/amd64` only.

## Build context

- Copy `.conanrc`, `.conan2/remotes.json`, `.conan2/profiles/default` and `conanfile.py` before `COPY . .`.
- Seed Conan from the `conan-cache` named build context: `FROM scratch AS conan-cache` for a local build, `conan-cache=.conan2` in `docker.yml`.
- Pass `SENTRY_AUTH_TOKEN` as the BuildKit secret `sentry-auth-token`, never as a build arg or env line.
- Keep `.git`, `tests/` and every git-tracked path in the build context.
- A git-tracked path listed in `.dockerignore` makes setuptools-scm see a dirty tree and version the wheel as a dev build.
- Exclude build outputs, virtual environments and caches, server data (`bedrock_server`, `data`) and untracked local config in `.dockerignore`.

## Runtime image

- Install `gosu`, `tzdata` and `libcurl4` with `--no-install-recommends`; BDS loads `libcurl.so.4`.
- Create the user and group `endstone` with uid and gid 1000 and give it `/data`.
- Keep `/data` the working directory and the only `VOLUME`.
- Install `entrypoint.sh` and `healthcheck.py` in `/usr/local/bin`, never under `/data`.
- Expose `19132/udp` and `19133/udp`.
- Run the default command as `endstone --server-folder /data --yes`.
- Default `PUID` and `PGID` to 1000 and `TZ` to `UTC`.

## Entrypoint and healthcheck

- `entrypoint.sh` remaps `endstone` to `PUID` and `PGID` with `usermod -o` and `groupmod -o`, chowns only the files in `/data` not owned by `endstone`, then runs `exec gosu endstone "$@"`.
- Started as non-root, `entrypoint.sh` runs `exec "$@"` unchanged.
- `healthcheck.py` is a stdlib-only RakNet unconnected ping to `127.0.0.1` that exits 0 only on a pong; `SERVER_PORT` overrides port 19132.
- Declare `HEALTHCHECK` with `--start-period=120s --interval=30s --timeout=5s --retries=5`, and keep the script's socket timeout below 5 s.

## Compose and registries

- `docker-compose.yml` builds the root `Dockerfile` as `endstone/endstone:latest`, maps both UDP ports, mounts `./data:/data` and passes `PUID`, `PGID` and `TZ`.
- Keep `stdin_open`, `tty` and `stop_grace_period: 15s` in Compose.
- Publish release images to `ghcr.io/endstonemc/endstone` and Docker Hub `endstone/endstone`, tagged `X.Y.Z` and, on a tag push, `latest`; the nightly goes to GHCR only as `:nightly`.
- `build.yml` pushes images only on a tag push; a `workflow_dispatch` or branch run builds without pushing.
- `docker.yml` builds the branch named in its checkout `ref`, only when that branch has a commit in the last 24 hours or the run is a manual dispatch, and only in `EndstoneMC/endstone`.
- Check out only `.github/docker` and `docker` in the `docker` job of `build.yml`.
- Add each new directory that feeds an image to the `changes` filter in `build.yml`.
- Docker workflows restore the Conan cache and never save it.
