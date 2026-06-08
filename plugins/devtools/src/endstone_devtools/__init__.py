# Intentionally empty — keep it that way.
#
# The GUI runs in a separate, spawned Python process that imports
# `endstone_devtools.gui`. Importing this package therefore must NOT pull in
# anything from `endstone` (the native server bindings), otherwise the GUI
# process would try to load the server runtime with no server attached.
#
# The plugin entry point targets `endstone_devtools.plugin` directly, so this
# module never needs to re-export the plugin class.
