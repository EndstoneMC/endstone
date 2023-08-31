from setuptools import find_packages, setup

from .plugin import Plugin
from .plugin_description import PluginDescriptionFile
from .plugin_loader import ZipPluginLoader, SourcePluginLoader


def plugin_setup(**attrs):
    """
    Custom setup function for plugins.

    Args:
        **attrs: Arbitrary keyword arguments for the 'setup' function from setuptools.

    Returns:
        Result of the 'setup' function from setuptools.
    """

    description_file = "plugin.toml"

    with open(description_file, "rb") as f:
        description = PluginDescriptionFile(f)

    name = attrs.pop("name", description.name.lower())
    version = attrs.pop("version", description.version)
    packages = attrs.pop("packages", find_packages())
    if "." not in packages:
        packages.append(".")

    package_data = attrs.pop("package_data", {})
    data_files = package_data.setdefault("", [])
    if description_file not in data_files:
        data_files.append(description_file)

    return setup(name=name, version=version, packages=packages, package_data=package_data, **attrs)
