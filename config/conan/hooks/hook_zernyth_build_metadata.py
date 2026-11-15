"""Normalize private paths in OpenSSL diagnostic metadata before compilation."""

import hashlib
import os
from pathlib import Path


SOURCE_SHA256 = "71449df12e0bb956cb8785bb370489a5d7fbce4b1b786980585244d6faef51a4"
ANCHOR = "my $cflags = join(' ', @ARGV);"
NORMALIZED = ANCHOR + r'''

# Zernyth normalizes diagnostic metadata; actual compiler arguments are unchanged.
for my $prefix ($ENV{'ZERNYTH_BUILD_PATH_ROOT'}, $ENV{'CONAN_HOME'}) {
    next unless defined($prefix) && length($prefix);
    $cflags =~ s/\Q$prefix\E/\/zernyth/g;
}'''


def pre_build(conanfile):
    """Apply the reviewed source-generator patch to the pinned OpenSSL build only."""
    if str(conanfile.name) != "openssl":
        return
    if str(conanfile.version) != "3.6.5":
        raise RuntimeError("Review the OpenSSL build-metadata hook before changing versions")
    if not os.environ.get("ZERNYTH_BUILD_PATH_ROOT") or not os.environ.get("CONAN_HOME"):
        raise RuntimeError("OpenSSL release builds require explicit private build/cache roots")
    source = Path(conanfile.source_folder) / "util/mkbuildinf.pl"
    current = source.read_text(encoding="utf-8")
    original = current.replace(NORMALIZED, ANCHOR)
    if hashlib.sha256(original.encode()).hexdigest() != SOURCE_SHA256:
        raise RuntimeError("OpenSSL metadata generator differs from the reviewed source")
    if current == original:
        source.write_text(original.replace(ANCHOR, NORMALIZED), encoding="utf-8", newline="\n")
