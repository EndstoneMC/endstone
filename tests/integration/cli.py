import argparse
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run the complete Endstone event smoke suite and return its result.")
    parser.add_argument(
        "--server-folder",
        type=Path,
        required=True,
        help="Folder containing (or to be populated with) the BDS executable.",
    )
    parser.add_argument(
        "--scenario",
        dest="scenarios",
        action="append",
        metavar="NAME",
        help=(
            "Run only the named automated event scenario; repeat the option to "
            "select multiple scenarios. Omit it for the complete suite."
        ),
    )
    return parser.parse_args()
