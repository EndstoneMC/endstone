---
name: analyze-profile
description: Decodes a spark profile from spark.lucko.me offline and maps its BDS frames to symbol names. Use when asked to read, decode or explain a spark profile or a server-thread hotspot.
---

# Analyze Profile

Read first: `.claude/rules/re/evidence.md`, `.claude/rules/re/symbols.md`.

1. Work in a scratch directory outside the repo and run Python that reads the data with `-I`.
2. Download the profile: `curl -sSfL https://spark-usercontent.lucko.me/<id> -o <scratch>/spark.bin`.
3. Generate bindings from `proto/spark/spark.proto` and `proto/spark/spark_sampler.proto` in `github.com/EndstoneMC/spark`: `uv run --no-project --with grpcio-tools python -I -m grpc_tools.protoc -I<proto-root> --python_out=<scratch>/gen <proto-root>/spark/spark.proto <proto-root>/spark/spark_sampler.proto`.
4. Write `<scratch>/decode.py` that inserts `<scratch>/gen` into `sys.path` and parses the file as `spark.SamplerData`, clearing `platform_statistics.world.worlds` before printing metadata.
5. Run it: `uv run --no-project --with protobuf python -I <scratch>/decode.py`.
6. Select the `MC_SERVER` thread and compute milliseconds per tick per window.
7. Map `bedrock_server::0x...` frames to names through `src/bedrock/symbols/linux.h` after matching the binary's SHA-256 against the zip fetched by `.claude/skills/upgrade-bds/fetch_bds.py`.
8. Report the hottest frames with their names, tick cost and call paths.
