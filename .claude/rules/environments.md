---
paths:
  - "tests/plugin/**"
  - ".github/workflows/**"
  - "CHANGELOG.md"
---

# Environments

- Endstone has three environments, and a release candidate moves through them in order: QA, UAT, then PROD.
- `test.endstone.dev:19132` is the front door for every hosted environment: endgate, which sends each join to a backend by the host the client typed, through its `[forced-hosts]`.
- QA is local: the `tests/plugin` harness against the project's `bedrock_server` on Windows, Linux and `--transport nethernet`, on ports 20232-20234, run by `validate-change`; every PR and every release candidate passes here first.
- QA servers on the test host take ports 20132-21131 and UAT servers 21132-22131; the front door sends `qa.endstone.dev` to 20132 and `uat.endstone.dev` to 21132.
- The remote dev environment on the test host runs one `ghcr.io/endstonemc/remote-dev` container per developer; when a change needs real clients, the developer starts a QA server from it on port 20132, reached as `qa.endstone.dev`, and stops it when done.
- UAT is `uat.endstone.dev`, a BDS in the UAT range of the test host behind endgate; it runs a release candidate wheel built from the `vX.Y` branch, and players on real clients accept it before the tag.
- PROD is not a server but what a release publishes, the PyPI wheels, the GHCR and Docker Hub images and the GitHub release, which server admins install; `release-version`'s publish checks are the PROD check.
- `devops-engineer` deploys the candidate to UAT and `qa-lead` signs UAT off.
- Each hosted backend is a Pterodactyl server whose egg runs `pip install -U` on every start, from `SERVER_WHLFILE` when set, else `ENDSTONE_VERSION` from PyPI, else the latest release.
- Deploy a candidate by uploading the wheel into the UAT server's volume, setting `SERVER_WHLFILE` to it, and restarting only that server; clear `SERVER_WHLFILE` after sign-off, and roll back by clearing it and pinning `ENDSTONE_VERSION` to the previous release.
- Build the candidate for the UAT container's Python, a manylinux x86_64 wheel, with a version no earlier candidate used, since pip skips a version already installed.
- Give every backend behind endgate endgate's `keys/forwarding_key.pem` as its `forwarding-key-file`, and keep ports 20132-22131 unreachable except through endgate.
- Serve NetherNet join requests over HTTPS from endgate's `tls-certificate`, since iOS clients join only over TLS signaling.
- Check the UAT and endgate logs for connected players before a restart.
- Give a minor at least 48 hours on UAT over 2 play sessions, and a patch 24 hours.
- Deploy to UAT only a wheel that passed QA, and never point the harness, a crash test or a benchmark at UAT or at the front door.
- Send a UAT failure back to QA as a failing `tests/plugin` scenario before fixing it.
- Restart UAT only to deploy a new candidate, and leave endgate, the other environments' servers and the host's panel, daemon and TLS proxy alone.
