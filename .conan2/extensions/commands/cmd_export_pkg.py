import os

from conan.api.output import ConanOutput
from conan.cli import make_abs_path
from conan.cli.args import add_lockfile_args, add_profiles_args, add_reference_args
from conan.cli.command import OnceArgument, conan_command
from conan.cli.formatters.graph import format_graph_json
from conan.errors import ConanException
from conan.internal.graph.graph import BINARY_BUILD, RECIPE_INCACHE
from conan.internal.methods import run_package_method


@conan_command(group="Creator", formatters={"json": format_graph_json})
def export_pkg(conan_api, parser, *args):
    """
    Create a package directly from pre-compiled binaries.
    """
    parser.add_argument(
        "path",
        help="Path to a folder containing a recipe (conanfile.py). Defaults to current directory",
        default=".",
        nargs="?",
    )
    parser.add_argument("-of", "--output-folder", help="The root output folder for generated and build files")
    parser.add_argument(
        "--build-require", action="store_true", default=False, help="Whether the provided reference is a build-require"
    )
    parser.add_argument(
        "-tf",
        "--test-folder",
        action=OnceArgument,
        help='Alternative test folder name. By default it is "test_package". Use "" to skip the test stage',
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "-r", "--remote", action="append", default=None, help="Look in the specified remote or remotes server"
    )
    group.add_argument(
        "-nr", "--no-remote", action="store_true", help="Do not use remote, resolve exclusively in the cache"
    )

    add_reference_args(parser)
    add_lockfile_args(parser)
    add_profiles_args(parser)
    args = parser.parse_args(*args)

    cwd = os.getcwd()
    path = conan_api.local.get_conanfile_path(args.path, cwd, py=True)
    overrides = eval(args.lockfile_overrides) if args.lockfile_overrides else None
    lockfile = conan_api.lockfile.get_lockfile(
        lockfile=args.lockfile, conanfile_path=path, cwd=cwd, partial=args.lockfile_partial, overrides=overrides
    )
    conan_api.lockfile.check_lockfile_config(lockfile)
    profile_host, profile_build = conan_api.profiles.get_profiles_from_args(args)
    remotes = conan_api.remotes.list(args.remote) if not args.no_remote else []
    output_folder = make_abs_path(args.output_folder, cwd) if args.output_folder else None

    # UX error check, for python-requires use "conan export" or "conan create"
    conanfile = conan_api.local.inspect(
        path, remotes, lockfile, name=args.name, version=args.version, user=args.user, channel=args.channel
    )
    # The package_type is not fully processed at export
    if conanfile.package_type == "python-require":
        raise ConanException("export-pkg can only be used for binaries, not for 'python-require'")

    # Compute the dependency graph to prepare the exporting of the package biary
    graph = conan_api.graph.load_graph_consumer(
        path=path,
        name=args.name,
        version=args.version,
        user=args.user,
        channel=args.channel,
        profile_host=profile_host,
        profile_build=profile_build,
        lockfile=lockfile,
        remotes=remotes,
        update=None,
        is_build_require=args.build_require,
    )
    graph.report_graph_error()
    conan_api.graph.analyze_binaries(graph, build_mode=["missing"], lockfile=lockfile, remotes=remotes)
    graph.report_graph_error()

    # Now export the final binary
    ConanOutput().title("Exporting package")
    hook_manager = conan_api._api_helpers.hook_manager
    package_folder = os.path.abspath(os.path.join(output_folder, "package"))
    pkg_node = graph.root
    ref = pkg_node.ref
    source_folder = os.path.dirname(pkg_node.path)
    out = ConanOutput(scope=pkg_node.conanfile.display_name)
    conanfile = pkg_node.conanfile

    conanfile.folders.set_base_folders(source_folder, output_folder)
    conanfile.folders.set_base_package(package_folder)

    prev = run_package_method(conanfile, pkg_node.package_id, hook_manager, ref)
    pkg_node.prev = prev
    pkg_node.recipe = RECIPE_INCACHE
    pkg_node.binary = BINARY_BUILD
    out.info(f"Package folder {package_folder}")
    out.success("Exported package binary")
    # conan_api.export.export_pkg(graph, output_folder)

    lockfile = conan_api.lockfile.update_lockfile(lockfile, graph, args.lockfile_packages, clean=args.lockfile_clean)
    conan_api.lockfile.save_lockfile(lockfile, args.lockfile_out, cwd)
    return {"graph": graph, "conan_api": conan_api}
