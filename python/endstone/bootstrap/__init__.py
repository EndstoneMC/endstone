import importlib.resources
import site
from pathlib import Path
from typing import Union

import endstone


def find_library(lib_filename: Union[str, Path]) -> Path:
    # try to find under site-packages folder
    for parent_dir in site.getsitepackages():
        lib_path = Path(parent_dir) / endstone.__package__ / lib_filename
        if lib_path.exists():
            return lib_path.absolute()

    # try to find under resources path
    with importlib.resources.path(endstone.__package__, lib_filename) as lib_path:
        if lib_path.exists():
            return lib_path.absolute()

    raise FileNotFoundError(
        f"Unable to find dynamic library: {str(lib_filename)}. Please ensure endstone is installed correctly."
    )
