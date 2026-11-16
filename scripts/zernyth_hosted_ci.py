"""Provision the explicitly approved dependencies on an ephemeral Windows CI runner."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import urllib.request
import zipfile


ROOT = Path(__file__).resolve().parents[1]
GRADLE_VERSION = "9.8.2"
GRADLE_SHA256 = "21bd5da2de49b642904077f38c0efdc7778e92b3ed0889392dca61b97e389607"


def run(command, cwd=ROOT):
    if Path(str(command[0])).suffix.lower() in (".bat", ".cmd"):
        command = 'cmd /d /s /c ""' + str(command[0]) + '" ' + subprocess.list2cmdline(
            list(map(str, command[1:]))) + '"'
    subprocess.run(command, cwd=cwd, check=True, timeout=2400)


def toolchain():
    """Select the installed LLVM toolchain and fingerprint its ABI environment."""
    candidates = [Path(os.environ.get("ProgramFiles", "C:/Program Files")) / "LLVM/bin"]
    found = shutil.which("clang-cl")
    if found:
        candidates.append(Path(found).parent)
    for folder in candidates:
        compiler = folder / "clang-cl.exe"
        if not compiler.is_file():
            continue
        output = subprocess.check_output([str(compiler), "--version"], text=True)
        version = re.search(r"clang version (\d+)", output)
        if version and int(version.group(1)) >= 20 and (folder / "lld-link.exe").is_file():
            break
    else:
        raise RuntimeError("The hosted Windows image requires installed LLVM/clang-cl 20 or newer")
    fingerprint = hashlib.sha256((output + sys.version + os.environ.get("VCToolsVersion", "")
                                  + os.environ.get("WindowsSDKVersion", "")).encode()).hexdigest()[:20]
    with Path(os.environ["GITHUB_PATH"]).open("a", encoding="utf-8") as stream:
        stream.write(str(folder) + "\n")
    with Path(os.environ["GITHUB_OUTPUT"]).open("a", encoding="utf-8") as stream:
        stream.write("fingerprint=" + fingerprint + "\n")
    temporary = Path(os.environ["RUNNER_TEMP"])
    settings = {"CONAN_HOME": temporary / "zernyth-conan",
                "GRADLE_USER_HOME": temporary / "zernyth-gradle-cache",
                "ZERNYTH_HOSTED_TOOLS": temporary / "zernyth-tools",
                "ZERNYTH_WHEELHOUSE": temporary / "zernyth-wheelhouse"}
    gradle = settings["ZERNYTH_HOSTED_TOOLS"] / f"gradle-{GRADLE_VERSION}/bin/gradle.bat"
    with Path(os.environ["GITHUB_ENV"]).open("a", encoding="utf-8") as stream:
        for key, value in settings.items():
            stream.write(key + "=" + str(value) + "\n")
        stream.write("ZERNYTH_GRADLE=" + str(gradle) + "\n")
    print(output.splitlines()[0])


def prepare():
    """Download pinned tools, then prepopulate caches consumed by the offline build."""
    if os.name != "nt" or os.environ.get("GITHUB_ACTIONS") != "true" or os.environ.get(
            "ZERNYTH_ALLOW_HOSTED_DOWNLOADS") != "1":
        raise RuntimeError("Dependency provisioning requires explicit approval in hosted Windows CI")
    temporary = Path(os.environ["RUNNER_TEMP"]).resolve()
    for name in ("CONAN_HOME", "GRADLE_USER_HOME", "ZERNYTH_HOSTED_TOOLS", "ZERNYTH_WHEELHOUSE"):
        path = Path(os.environ[name])
        if path.is_symlink() or not path.resolve().is_relative_to(temporary):
            raise RuntimeError("Hosted build directories must remain inside RUNNER_TEMP")
    wheelhouse = Path(os.environ["ZERNYTH_WHEELHOUSE"])
    wheelhouse.mkdir(parents=True, exist_ok=True)
    requirements = ROOT / "config/ci/windows-python-requirements.txt"
    # PyYAML 6.0.3 has no CPython 3.15 Windows wheel. Build its pinned source
    # distribution with the selected compiler while retaining wheels for all other packages.
    run([sys.executable, "-m", "pip", "wheel", "--only-binary=:all:", "--no-binary=pyyaml",
         "--wheel-dir", str(wheelhouse), "-r", str(requirements)])
    run([sys.executable, "-m", "pip", "install", "--no-index", "--find-links",
         str(wheelhouse), "-r", str(requirements)])
    tools = Path(os.environ["ZERNYTH_HOSTED_TOOLS"])
    tools.mkdir(parents=True, exist_ok=True)
    gradle = tools / f"gradle-{GRADLE_VERSION}/bin/gradle.bat"
    if not gradle.is_file():
        archive = tools / f"gradle-{GRADLE_VERSION}-bin.zip"
        urllib.request.urlretrieve(
            f"https://services.gradle.org/distributions/gradle-{GRADLE_VERSION}-bin.zip", archive)
        with archive.open("rb") as stream:
            if hashlib.file_digest(stream, "sha256").hexdigest() != GRADLE_SHA256:
                raise RuntimeError("Gradle distribution checksum mismatch")
        with zipfile.ZipFile(archive) as distribution:
            distribution.extractall(tools)
        archive.unlink()
    home = Path(os.environ["CONAN_HOME"])
    (home / "profiles").mkdir(parents=True, exist_ok=True)
    shutil.copyfile(ROOT / "config/conan/windows-hosted.profile", home / "profiles/default")
    # The pinned Funchook and Mojang RakNet recipes are provided by the
    # repository's existing Endstone remote, alongside Conan Center.
    shutil.copyfile(ROOT / ".conan2/remotes.json", home / "remotes.json")
    conan = shutil.which("conan")
    if not conan:
        raise RuntimeError("Pinned Conan installation is missing")
    command = [conan, "install", str(ROOT), "--output-folder=" + str(ROOT), "--build=missing",
               "--lockfile=" + str(ROOT / "config/zernyth-publish-conan.lock"),
               "-pr:h", "default", "-pr:h", str(ROOT / "config/conan/windows-sanitized.profile"),
               "-s", "&:build_type=RelWithDebInfo"]
    if os.environ.get("ZERNYTH_CONAN_CACHE_HIT") != "true":
        command += ["--build=sentry-native/*", "--build=cpptrace/*", "--build=raknet/*"]
    run(command, cwd=ROOT.parent)
    init = tools / "resolve-dependencies.gradle"
    init.write_text('''gradle.projectsEvaluated {
    rootProject.tasks.register("zernythResolveHostedDependencies") {
        doLast {
            rootProject.allprojects.each { project ->
                project.configurations.findAll { it.canBeResolved }.each { configuration ->
                    configuration.incoming.artifactView {
                        componentFilter { id ->
                            id instanceof org.gradle.api.artifacts.component.ModuleComponentIdentifier
                        }
                    }.files.files
                }
            }
        }
    }
}
''', encoding="utf-8")
    run([str(gradle), "--no-daemon", "--init-script", str(init),
         "zernythResolveHostedDependencies"], cwd=ROOT / "java")
    run([conan, "cache", "clean", "*", "--build", "--download", "--temp"], cwd=ROOT.parent)
    inventory = {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(wheelhouse.glob("*.whl"))}
    (wheelhouse / "SHA256.json").write_text(json.dumps(inventory, indent=2) + "\n", encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("toolchain", "prepare"))
    args = parser.parse_args()
    {"toolchain": toolchain, "prepare": prepare}[args.command]()


if __name__ == "__main__":
    main()
