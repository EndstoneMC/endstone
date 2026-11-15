"""Package the existing Java SDK artifacts without building or downloading anything."""

import argparse
import hashlib
import os
from pathlib import Path
import shutil
import tempfile
import zipfile

import zernyth_release as release


ROOT = Path(__file__).resolve().parents[1]


def package(output):
    """Create a deterministic SDK from matched API/publication/example artifacts."""
    version = release.version()
    output = release.workspace(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="zernyth-sdk-", dir=output.parent) as temporary:
        stage = Path(temporary) / f"zernyth-sdk-{version}"
        stage.mkdir()

        def copy(source, relative):
            destination = stage / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            if source.is_symlink():
                raise ValueError("SDK source must be a regular file")
            shutil.copyfile(source, destination)

        api = ROOT / f"java/api/build/libs/api-{version}.jar"
        example = ROOT / f"java/example/build/libs/example-{version}.jar"
        for jar in (api, example):
            if release.jar_version(jar) != version:
                raise ValueError("SDK artifact version mismatch")
        with zipfile.ZipFile(api) as jar:
            header = jar.read("dev/zernyth/api/Server.class")[:8]
            if header[:4] != b"\xca\xfe\xba\xbe" or int.from_bytes(header[6:8], "big") != 69:
                raise ValueError("The SDK requires Java 25 bytecode")
        publication = ROOT / "java/build/sdk-repository/dev/zernyth/api" / version
        for suffix in (".jar", "-sources.jar", "-javadoc.jar"):
            source = ROOT / f"java/api/build/libs/api-{version}{suffix}"
            if release.digest(source) != release.digest(publication / source.name):
                raise ValueError("SDK publication and API artifacts differ")
            copy(source, "api/" + source.name)
        for source in publication.iterdir():
            copy(source, f"sdk-repository/dev/zernyth/api/{version}/" + source.name)
        template = ROOT / "zernyth-plugin-template"
        for name in ("README.md", "build.gradle.kts", "settings.gradle.kts"):
            copy(template / name, "zernyth-plugin-template/" + name)
        for base, destination in ((template / "src", "zernyth-plugin-template/src"),
                                  (ROOT / "java/example/src", "examples/hello/src")):
            for source in base.rglob("*"):
                if source.is_file() and source.suffix in (".java", ".yml"):
                    copy(source, destination + "/" + source.relative_to(base).as_posix())
        copy(example, "examples/hello/" + example.name)
        for name in ("LICENSE", "THIRD_PARTY_NOTICES.md"):
            copy(ROOT / name, name)
        copy(ROOT / "packaging/sdk/README.md", "README.md")
        archive = Path(temporary) / "sdk.zip"
        release.reproducible_zip(stage, archive)
        os.link(archive, output)
    with output.open("rb") as stream:
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
    print("PASS", output.name, digest)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    package(args.output)


if __name__ == "__main__":
    main()
