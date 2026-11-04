import enum
import importlib
import os
import pkgutil
import sys
import threading
import traceback
import types
from dataclasses import dataclass

from endstone_test.harness.control import Control

STATE = "_endstone_test_api_recorder"
MODE_ENV = "ENDSTONE_TEST_API_GATE"
CONTROL_ENV = "ENDSTONE_TEST_CONTROL"
RECORDING_MODES = ("enforce", "report")
TOOL_IDS = (3, 4)
TOOL_NAME = "endstone-test-api-recorder"
SKIP_SUBMODULES = {"cli"}
SKIP_DUNDERS = {"__new__", "__init_subclass__", "__class_getitem__", "__subclasshook__"}
STATIC_PROPERTY = "pybind11_static_property"
INSTANCE_METHOD = "instancemethod"
FRAMEWORK = "framework"
PASS_THROUGH = {"endstone", "recorder"}
KEEP_CALL_SITES = {"scenario", "pytest", "fixture", "plugin"}
CATEGORIES = (
    ("/endstone_test/harness/api_recorder.py", "recorder"),
    ("/endstone_test/scenarios/", "scenario"),
    ("/endstone_test/tests/", "pytest"),
    ("/endstone_test/harness/", "harness"),
    ("/endstone_test/", "plugin"),
    ("/endstone_test_fixture/", "fixture"),
    ("/endstone_test_addon/", "fixture"),
    ("/endstone/tests/test_", "repo-tests"),
    ("/endstone/", "endstone"),
)
DISPATCHED = {
    "endstone._python.plugin.Plugin": ("on_load", "on_enable", "on_disable"),
    "endstone._python.plugin.PluginLoader": ("load_plugin", "load_plugins", "enable_plugin", "disable_plugin"),
    "endstone._python.command.Command": ("execute",),
    "endstone._python.command.CommandExecutor": ("on_command",),
    "endstone._python.map.MapRenderer": ("initialize", "render"),
    "endstone._python.metrics.CustomChart": ("get_chart_data",),
}


@dataclass(frozen=True)
class Member:
    key: str
    kind: str
    owner: object
    name: str
    impl: object
    bound: bool


def qualified(cls: type) -> str:
    return f"{cls.__module__}.{cls.__qualname__}"


def api_modules() -> list[types.ModuleType]:
    import endstone

    names = ["endstone"]
    for info in sorted(pkgutil.iter_modules(endstone.__path__), key=lambda i: i.name):
        if info.ispkg and not info.name.startswith("_") and info.name not in SKIP_SUBMODULES:
            names.append(f"endstone.{info.name}")
    return [importlib.import_module(name) for name in names]


def binding_modules() -> list[types.ModuleType]:
    import endstone._python as root

    found, pending = [root], [root]
    while pending:
        for value in vars(pending.pop()).values():
            if (
                isinstance(value, types.ModuleType)
                and value.__name__.startswith("endstone._python")
                and value not in found
            ):
                found.append(value)
                pending.append(value)
    return found


def is_enum(cls: type) -> bool:
    return issubclass(cls, enum.Enum)


def api_classes() -> tuple[list[tuple[str, type]], list[tuple[str, object]]]:
    seen: set[int] = set()
    classes: list[tuple[str, type]] = []
    functions: list[tuple[str, object]] = []

    def add(path: str, cls: type) -> None:
        if id(cls) not in seen and not is_enum(cls):
            seen.add(id(cls))
            classes.append((path, cls))

    for module in api_modules():
        for name in getattr(module, "__all__", ()):
            value = getattr(module, name, None)
            if isinstance(value, type):
                add(f"{module.__name__}.{name}", value)
            elif isinstance(value, (types.FunctionType, types.BuiltinFunctionType)):
                functions.append((f"{module.__name__}.{name}", value))
    for module in binding_modules():
        for name, value in vars(module).items():
            if isinstance(value, type) and not name.startswith("_"):
                add(f"{module.__name__}.{name}", value)
    index = 0
    while index < len(classes):
        path, cls = classes[index]
        index += 1
        for name, value in vars(cls).items():
            if isinstance(value, type) and not name.startswith("_"):
                add(f"{path}.{name}", value)
        for base in cls.__mro__[1:]:
            if base.__module__.startswith("endstone"):
                add(qualified(base), base)
    return classes, functions


def dispatched(cls: type, name: str) -> bool:
    return name in DISPATCHED.get(qualified(cls), ())


def class_members(path: str, cls: type, unwrap) -> list[Member]:
    members = []
    for name, value in vars(cls).items():
        dunder = name.startswith("__") and name.endswith("__")
        if (name.startswith("_") and not dunder) or name in SKIP_DUNDERS:
            continue
        key = f"{path}.{name}"
        if isinstance(value, staticmethod):
            members.append(Member(key, "static", cls, name, unwrap(value.__func__), False))
        elif isinstance(value, classmethod):
            members.append(Member(key, "classmethod", cls, name, unwrap(value.__func__), False))
        elif isinstance(value, property):
            if type(value).__name__ == STATIC_PROPERTY:
                continue
            if value.fget is not None:
                members.append(Member(f"{key}#get", "getter", cls, name, unwrap(value.fget), True))
            if value.fset is not None:
                members.append(Member(f"{key}#set", "setter", cls, name, unwrap(value.fset), True))
        else:
            impl = value.__func__ if type(value).__name__ == INSTANCE_METHOD else value
            if not isinstance(impl, (types.FunctionType, types.BuiltinFunctionType)):
                continue
            kind = "constructor" if name == "__init__" else "operator" if dunder else "method"
            members.append(Member(key, kind, cls, name, unwrap(impl), True))
    return members


def api_members(unwrap=lambda fn: fn) -> list[Member]:
    classes, functions = api_classes()
    members = []
    for path, cls in classes:
        members += class_members(path, cls, unwrap)
    for path, function in functions:
        members.append(Member(path, "function", None, path.rsplit(".", 1)[1], unwrap(function), False))
    return members


class Recorder:
    def __init__(self):
        self.lock = threading.RLock()
        self.seen: set[tuple[str, str]] = set()
        self.hits: dict[str, set[str]] = {}
        self.unsent: dict[str, set[str]] = {}
        self.errors = 0
        self.last_error = ""
        self.categories: dict[str, str] = {}
        self.members: list[Member] = []
        self.wrapped: dict[str, Member] = {}
        self.codes: dict[int, list[Member]] = {}
        self.calls: dict[int, Member] = {}
        self.overrides: dict[str, list[tuple[type, str]]] = {}
        self.keep: list[object] = []
        self.problems: list[str] = []
        self.wrapper_code = None
        self.tool = None
        self.disable = None
        self.instancemethod = None

    def unwrap(self, fn):
        if isinstance(fn, types.FunctionType) and fn.__code__ is self.wrapper_code:
            return fn.__wrapped__
        return fn

    def category(self, filename: str) -> str:
        found = self.categories.get(filename)
        if found is None:
            path = filename.replace("\\", "/")
            found = next((name for marker, name in CATEGORIES if marker in path), "other")
            self.categories[filename] = found
        return found

    def source(self, frame) -> str:
        while frame is not None:
            found = self.category(frame.f_code.co_filename)
            if found not in PASS_THROUGH:
                return found
            frame = frame.f_back
        return FRAMEWORK

    def hit(self, key: str, frame) -> None:
        try:
            pair = (key, self.source(frame))
            if pair in self.seen:
                return
            with self.lock:
                if pair in self.seen:
                    return
                self.seen.add(pair)
                self.hits.setdefault(key, set()).add(pair[1])
                self.unsent.setdefault(key, set()).add(pair[1])
        except BaseException as e:  # noqa: BLE001
            self.failed(e)

    def failed(self, error: BaseException) -> None:
        self.errors += 1
        self.last_error = f"{type(error).__name__}: {error}"

    def take(self) -> tuple[dict[str, list[str]], int, str]:
        with self.lock:
            unsent, self.unsent = self.unsent, {}
        return {key: sorted(sources) for key, sources in unsent.items()}, self.errors, self.last_error

    def wrapper(self, impl, key: str):
        hit = self.hit
        getframe = sys._getframe

        def recorded(*args, **kwargs):
            __tracebackhide__ = True
            hit(key, getframe(1))
            return impl(*args, **kwargs)

        for attribute in ("__name__", "__qualname__", "__doc__", "__module__"):
            try:
                setattr(recorded, attribute, getattr(impl, attribute))
            except (AttributeError, TypeError):
                pass
        recorded.__wrapped__ = impl
        return recorded

    def install(self) -> None:
        self.wrapper_code = self.wrapper(len, "").__code__
        self.members = api_members()
        by_attribute: dict[tuple[int, str], list[Member]] = {}
        for member in self.members:
            if isinstance(member.impl, types.FunctionType):
                self.codes.setdefault(id(member.impl.__code__), []).append(member)
                self.keep.append(member.impl.__code__)
            elif member.owner is None or dispatched(member.owner, member.name):
                self.calls[id(member.impl)] = member
                self.keep.append(member.impl)
            else:
                by_attribute.setdefault((id(member.owner), member.name), []).append(member)
        for path, names in DISPATCHED.items():
            owner = next((m.owner for m in self.members if m.owner is not None and qualified(m.owner) == path), None)
            for name in names:
                value = vars(owner).get(name) if owner is not None else None
                if type(value).__name__ == INSTANCE_METHOD:
                    self.instancemethod = type(value)
                    value = value.__func__
                member = self.calls.get(id(value))
                if member is None or member.owner is not owner:
                    self.problems.append(f"the trampoline table names {path}.{name}, which the bindings do not define")
                    continue
                self.overrides.setdefault(name, []).append((owner, member.key))
        for members in by_attribute.values():
            self.wrap(members)
        self.start_monitoring()
        self.scan_running_frames()

    def wrap(self, members: list[Member]) -> None:
        owner, name = members[0].owner, members[0].name
        value = vars(owner)[name]
        if isinstance(value, property):
            accessors = {m.kind: m for m in members}
            getter = accessors.get("getter")
            setter = accessors.get("setter")
            fget = self.wrapper(value.fget, getter.key) if getter else value.fget
            fset = self.wrapper(value.fset, setter.key) if setter else value.fset
            replacement = type(value)(fget, fset, value.fdel, value.__doc__)
        elif isinstance(value, (staticmethod, classmethod)):
            replacement = type(value)(self.wrapper(value.__func__, members[0].key))
        elif type(value).__name__ == INSTANCE_METHOD:
            replacement = type(value)(self.wrapper(value.__func__, members[0].key))
        else:
            for member in members:
                self.calls[id(member.impl)] = member
                self.keep.append(member.impl)
            return
        setattr(owner, name, replacement)
        if vars(owner)[name] is not replacement or type(vars(owner)[name]) is not type(value):
            self.problems.append(f"{members[0].key} could not be instrumented")
            return
        for member in members:
            self.wrapped[member.key] = member

    def start_monitoring(self) -> None:
        monitoring = sys.monitoring
        for tool in TOOL_IDS:
            if monitoring.get_tool(tool) is None:
                monitoring.use_tool_id(tool, TOOL_NAME)
                self.tool = tool
                break
        if self.tool is None:
            self.problems.append(f"no free sys.monitoring tool id among {TOOL_IDS}")
            return
        self.disable = monitoring.DISABLE
        monitoring.register_callback(self.tool, monitoring.events.PY_START, self.on_start)
        monitoring.register_callback(self.tool, monitoring.events.CALL, self.on_call)
        monitoring.set_events(self.tool, monitoring.events.PY_START | monitoring.events.CALL)

    def scan_running_frames(self) -> None:
        for frame in list(sys._current_frames().values()):
            while frame is not None:
                self.started(frame.f_code, frame)
                frame = frame.f_back

    def started(self, code, frame) -> bool:
        members = self.codes.get(id(code))
        if members is not None:
            for member in members:
                if not member.bound or self.owned(member.owner, code, frame):
                    self.hit(member.key, frame.f_back)
            return True
        candidates = self.overrides.get(code.co_name)
        if candidates and code.co_argcount:
            target = frame.f_locals.get(code.co_varnames[0])
            mro = type(target).__mro__
            keys = [key for base, key in candidates if base in mro]
            if keys and any(
                isinstance(vars(klass).get(code.co_name), types.FunctionType)
                and vars(klass)[code.co_name].__code__ is code
                for klass in mro
            ):
                for key in keys:
                    self.hit(key, frame.f_back)
                return True
        return False

    def owned(self, owner: type, code, frame) -> bool:
        if not code.co_argcount:
            return False
        return owner in type(frame.f_locals.get(code.co_varnames[0])).__mro__

    def on_start(self, code, offset):
        try:
            if self.started(code, sys._getframe(1)):
                return None
        except BaseException as e:  # noqa: BLE001
            self.failed(e)
        return self.disable

    def on_call(self, code, offset, target, arg0):
        try:
            kind = type(target)
            function = target.__func__ if kind is types.MethodType or kind is self.instancemethod else target
            member = self.calls.get(id(function))
            if member is not None and member.impl is function:
                self.hit(member.key, sys._getframe(1))
                return None
            if self.category(code.co_filename) in KEEP_CALL_SITES:
                return None
        except BaseException as e:  # noqa: BLE001
            self.failed(e)
        return self.disable


def recording_mode() -> str | None:
    mode = os.environ.get(MODE_ENV)
    if os.environ.get(CONTROL_ENV) and mode in RECORDING_MODES:
        return mode
    return None


def active_recorder() -> Recorder | None:
    return getattr(sys, STATE, None)


def install_from_env() -> Recorder | None:
    recorder = active_recorder()
    if recorder is None and recording_mode() is not None:
        recorder = Recorder()
        setattr(sys, STATE, recorder)
        try:
            recorder.install()
        except Exception:  # noqa: BLE001
            recorder.problems.append(f"the recorder failed to install:\n{traceback.format_exc()}")
    return recorder


class RecordingControl(Control):
    FLUSH_BEFORE = ("end", "alive", "done")

    def __init__(self, address: str, recorder: Recorder | None):
        super().__init__(address)
        self.recorder = recorder

    def send(self, message: dict) -> None:
        if self.recorder is not None and not self.closed and message.get("type") in self.FLUSH_BEFORE:
            hits, errors, last_error = self.recorder.take()
            if hits or errors:
                super().send({"type": "api_hits", "hits": hits, "errors": errors, "last_error": last_error})
        super().send(message)
