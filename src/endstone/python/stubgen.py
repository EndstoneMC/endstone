#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = [
#     "griffe>=2.1,<3",
# ]
# ///

"""
stubgen.py: pybind11 stub generation tool (griffe-backed)

This is a rewrite of nanobind's ``stubgen.py``. It keeps nanobind's overall
design -- a ``StubGen`` class with ``put_*`` methods that walk a module and
emit ``.pyi`` text *directly* into a buffer, but replaces nanobind's object
introspection with griffe.
"""

import argparse
import ast
import inspect
import io
import re
import sys
import textwrap
from dataclasses import dataclass
from pathlib import Path
from re import Pattern

import griffe
from griffe import (
    Alias,
    Attribute,
    Class,
    Extension,
    Function,
    Inspector,
    Kind,
    Module,
    Object,
    ObjectNode,
    ParameterKind,
)

# Various standard attributes found in modules, classes, etc.
SKIP_LIST = {
    "__doc__",
    "__module__",
    "__name__",
    "__new__",
    "__builtins__",
    "__cached__",
    "__path__",
    "__spec__",
    "__loader__",
    "__package__",
    "__class__",
    "__class_getitem__",
    "__orig_bases__",
    "__file__",
    "__dict__",
    "__weakref__",
    "__format__",
    "__firstlineno__",
    "__static_attributes__",
    "__annotations__",
    "__annotate__",
    "__annotate_func__",
    "__qualname__",
    "_pybind11_conduit_v1_",
    # Auto-generated enum attributes synthesized by type checkers.
    "_new_member_",
    "_use_args_",
    "_member_names_",
    "_member_map_",
    "_value2member_map_",
    "_hashable_values_",
    "_unhashable_values_",
    "_unhashable_values_map_",
    "_value_repr_",
    "_generate_next_value_",
}

# pybind11 native-enum value repr, e.g. <GameMode.ADVENTURE: 2>.
_ENUM_RE = re.compile(r"<(?P<enum>\w+(?:\.\w+)+): (?P<value>-?\d+)>")

# A pybind11 docstring signature line: "[N. ][name](args)[ -> ret]".
_PYBIND_SIG_RE = re.compile(
    r"^\s*(?:(?P<num>\d+)\.\s+)?(?P<name>[A-Za-z_]\w*)?\((?P<args>.*)\)\s*(?:->\s*(?P<ret>.+?))?\s*$"
)


# An enum-member default in a pybind11 signature: "<annotation> = <Enum.MEMBER: N>".
_ENUM_DEFAULT_RE = re.compile(r"(?P<ann>[\w.]+)(?P<eq> = )<(?P<enum>\w+(?:\.\w+)+): -?\d+>")

# An in-place operator and the binary one Python falls back to when it returns
# NotImplemented, which pybind11's narrower overload does.
_IN_PLACE_OPS = {
    "__iadd__": "__add__",
    "__isub__": "__sub__",
    "__imul__": "__mul__",
    "__itruediv__": "__truediv__",
}

# Dunders object already supplies.
_REDUNDANT_DUNDERS = {"__repr__", "__str__"}

_LITERAL_TYPES = (bool, int, float, str, bytes)


def _elevate_enum_default(m: "re.Match[str]") -> str:
    # Rebind a bare enum default to its parameter's fully-qualified type, so
    # simplify() shortens it like any other in-package name (nanobind arrives
    # here via type_str, which fully-qualifies the default before simplifying).
    ann, enum = m.group("ann"), m.group("enum")
    cls, _, member = enum.rpartition(".")
    if ann.rpartition(".")[2] == cls:
        return f"{ann}{m.group('eq')}{ann}.{member}"
    return f"{ann}{m.group('eq')}{enum}"


def _clean_sig_args(name: str | None, args: str) -> str:
    args = _ENUM_DEFAULT_RE.sub(_elevate_enum_default, args)
    args = _ENUM_RE.sub(r"\g<enum>", args)
    if name is None:
        return re.sub(r"^arg0:\s*[\w.]+", "self", args)
    return re.sub(r"^self:\s*[\w.]+", "self", args)


def _is_optional(ann: ast.expr) -> bool:
    if isinstance(ann, ast.Constant):
        return ann.value is None
    if isinstance(ann, ast.BinOp) and isinstance(ann.op, ast.BitOr):
        return _is_optional(ann.left) or _is_optional(ann.right)
    if isinstance(ann, ast.Subscript):
        name = ast.unparse(ann.value).rpartition(".")[2]
        if name == "Optional":
            return True
        if name == "Union" and isinstance(ann.slice, ast.Tuple):
            return any(_is_optional(e) for e in ann.slice.elts)
    return False


def _make_sig(name: str | None, args: str, ret: str) -> str:
    """Build "def (args) -> ret" from a pybind11 signature line, splicing in what it
    leaves out: a type for an untyped parameter, and the ``| None`` PEP 484 requires
    beside a ``None`` default."""
    sig = f"def ({_clean_sig_args(name, args)}) -> {ret.strip()}"
    src = f"def _{sig[4:]}: ..."
    # pybind11 docstrings can carry non-Python types (e.g. C++ "endstone::Foo").
    try:
        node = ast.parse(src).body[0]
    except SyntaxError:
        raise ValueError(f"unparseable signature {sig[4:]!r}") from None
    a = node.args
    positional = a.posonlyargs + a.args
    defaults = dict(zip(positional[len(positional) - len(a.defaults) :], a.defaults))
    defaults.update({p: d for p, d in zip(a.kwonlyargs, a.kw_defaults) if d is not None})
    edits: list[tuple[int, str]] = []
    for arg in positional + a.kwonlyargs + [a.vararg, a.kwarg]:
        if arg is None:
            continue
        default = defaults.get(arg)
        if arg.annotation is None:
            if arg.arg != "self":
                edits.append((arg.end_col_offset, ": typing.Any"))
        elif isinstance(default, ast.Constant) and default.value is None and not _is_optional(arg.annotation):
            edits.append((arg.annotation.end_col_offset, " | None"))
    for col, text in sorted(edits, reverse=True):
        src = src[:col] + text + src[col:]
    return "def " + src[5:-5]


def _self_return(sig: str) -> str:
    return sig[: sig.rindex(") -> ") + 5] + "typing.Self"


def _is_enum_member(owner) -> bool:
    return (
        owner is not None
        and owner.parent is not None
        and owner.parent.kind == Kind.CLASS
        and any("enum." in str(b) for b in owner.parent.bases)
    )


def _literal_type(value: str) -> str | None:
    """Name a rendered literal's type, so a bare value carries an annotation."""
    try:
        literal = ast.literal_eval(value)
    except (SyntaxError, ValueError):
        return None
    return type(literal).__name__ if isinstance(literal, _LITERAL_TYPES) else None


def is_private(name: str) -> bool:
    return len(name) > 2 and ((name[0] == "_" and name[1] != "_") or (name[-1] == "_" and name[-2] != "_"))


def _local_class_names(mod: Module) -> set:
    # Classes defined in the module, including nested ones. pybind11 refers to
    # same-module classes/enums by short name, so these must never be imported.
    names: set = set()
    stack = [mod]
    while stack:
        obj = stack.pop()
        for name, member in obj.members.items():
            if not isinstance(member, Alias) and member.kind == Kind.CLASS:
                names.add(name)
                stack.append(member)
    return names


def _extract_signatures(docstring) -> list[tuple] | None:
    """Parse pybind11 signature(s) from a docstring, mirroring nanobind's __nb_signature__.

    Returns a list of (sig, doc) where sig is "def (args) -> ret"; pybind11
    inlines default values into the signature text.
    """
    lines = docstring.lines
    if not lines:
        return None
    head = _PYBIND_SIG_RE.match(lines[0])
    if head is None:
        return None
    if len(lines) >= 2 and lines[1].strip() == "Overloaded function.":
        entries: list[tuple] = []
        doc_start = 2
        for i in range(2, len(lines)):
            m = _PYBIND_SIG_RE.match(lines[i])
            if m and m.group("ret") and m.group("num") == str(len(entries) + 1):
                if entries:
                    entries[-1] = (entries[-1][0], "\n".join(lines[doc_start:i]).strip() or None)
                sig = _make_sig(m.group("name"), m.group("args"), m.group("ret"))
                entries.append((sig, None))
                doc_start = i + 1
        if not entries:
            return None
        entries[-1] = (entries[-1][0], "\n".join(lines[doc_start:]).strip() or None)
        return entries
    if head.group("ret") is None:
        return None
    sig = _make_sig(head.group("name"), head.group("args"), head.group("ret"))
    return [(sig, "\n".join(lines[1:]).strip() or None)]


class Pybind11Support(Extension):
    """Umbrella griffe extension for pybind11 quirks"""

    def __init__(self) -> None:
        super().__init__()
        self.errors: list[str] = []

    def on_module_instance(self, *, node, mod: Module, agent, **kwargs) -> None:
        """Force griffe to inspect pybind11 submodules that share the parent's binary."""
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return
        for child in node.children:
            if child.alias_target_path is None or not child.is_module:
                continue
            if child.alias_target_path != f"{agent.current.path}.{child.name}":
                continue
            if getattr(child.obj, "__file__", None) == getattr(node.obj, "__file__", None):
                delattr(child.obj, "__file__")

    def on_members(self, *, node, obj: Object, agent, **kwargs) -> None:
        """Restore ``__dict__`` (definition) order; griffe's ``getmembers`` sorts alphabetically."""
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return
        ordered = {}
        for k in node.obj.__dict__:
            if k in obj.members:
                ordered[k] = obj.members.pop(k)
        obj.members.update(ordered)

    def on_class_members(self, *, node, cls: Class, agent, **kwargs) -> None:
        """Retype the operators pybind11 types by their C++ operand."""
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return
        # The data model requires these to take any object, and not to overload.
        for name in ("__eq__", "__ne__"):
            func = cls.members.get(name)
            if isinstance(func, Function):
                func._pybind11_signature_ = [("def (self, other: object) -> bool", None)]
                func.docstring = None
        # An in-place operator takes what the binary one it falls back to takes,
        # and returns the object it was called on.
        for iop, op in _IN_PLACE_OPS.items():
            ifunc = cls.members.get(iop)
            if not isinstance(ifunc, Function):
                continue
            func = cls.members.get(op)
            sigs = getattr(func, "_pybind11_signature_", None) or getattr(ifunc, "_pybind11_signature_", None)
            if sigs:
                ifunc._pybind11_signature_ = [(_self_return(s), None) for s, _ in sigs]

    def on_attribute_instance(self, *, node, attr: Attribute, agent, **kwargs) -> None:
        """Strip inherited docstrings and attach property setters/deleters."""
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return
        # Drop docstrings inherited from the value's type.
        if attr.docstring is not None and attr.docstring.value == type(node.obj).__doc__:
            attr.docstring = None
        # griffe records only the getter; rebuild setter/deleter from the descriptor.
        if "property" not in attr.labels:
            return
        fget = getattr(node.obj, "fget", None)
        if fget is not None and fget.__doc__:
            m = _PYBIND_SIG_RE.match(fget.__doc__.splitlines()[0])
            if m is not None and m.group("ret"):
                # Keep the fully-qualified string; assigning attr.annotation lets
                # griffe round-trip it (str -> Expr -> str) and drop the module
                # prefix, so a cross-module type like endstone._python.command.Command
                # would collapse to a bare, unresolved "Command".
                attr._pybind11_annotation_ = m.group("ret").strip()
        if fset := getattr(node.obj, "fset", None):
            fset_node = ObjectNode(fset, node.name, node)
            attr.setter = Function(name=node.name, docstring=agent._get_docstring(fset_node), parent=agent.current)
            attr.labels.add("writable")
            agent.extensions.call("on_instance", node=fset_node, obj=attr.setter, agent=agent)
            agent.extensions.call("on_function_instance", node=fset_node, func=attr.setter, agent=agent)
        if fdel := getattr(node.obj, "fdel", None):
            fdel_node = ObjectNode(fdel, node.name, node)
            attr.deleter = Function(name=node.name, docstring=agent._get_docstring(fdel_node), parent=agent.current)
            attr.labels.add("deletable")
            agent.extensions.call("on_instance", node=fdel_node, obj=attr.deleter, agent=agent)
            agent.extensions.call("on_function_instance", node=fdel_node, func=attr.deleter, agent=agent)

    def on_function_instance(self, *, node, func: Function, agent, **kwargs) -> None:
        """Parse the docstring signature(s) into func._pybind11_signature_ (see __nb_signature__)."""
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return
        if not func.docstring:
            return
        try:
            sigs = _extract_signatures(func.docstring)
        except ValueError as e:
            self.errors.append(f"{func.path}: {e}")
            return
        if sigs is not None:
            func._pybind11_signature_ = sigs
            func.docstring = None


def load(module_name: str) -> Module:
    """Load a module into a griffe tree with the pybind11 loading plumbing."""
    support = Pybind11Support()
    module = griffe.load(module_name, extensions=griffe.load_extensions(support))
    # Hooks collect errors here rather than raise; griffe mangles exceptions
    # thrown inside a hook into a generic ImportError.
    if support.errors:
        raise ValueError(f"{module_name}:\n  " + "\n  ".join(support.errors))
    if not isinstance(module, Module):
        raise ValueError(f"{module_name!r} is not a module")
    return module


# --------------------------------------------------------------------------- #
#  Pattern files                                                               #
# --------------------------------------------------------------------------- #


@dataclass
class ReplacePattern:
    query: Pattern[str]
    lines: list[str]
    matches: int


def load_pattern_file(fname: str) -> list[ReplacePattern]:
    """Load a pattern file; see nanobind's stubgen docs for the syntax."""
    with open(fname, encoding="utf-8") as f:
        f_lines = f.readlines()

    patterns: list[ReplacePattern] = []

    def add_pattern(query: str, lines: list[str]) -> None:
        # The body is emitted verbatim, trailing blank line included: one that
        # replaces a class or a function needs that separator, one that replaces
        # a value does not, and only the author knows which.
        if all(p.isspace() or len(p) == 0 for p in lines):
            lines = []
        patterns.append(ReplacePattern(re.compile(query[:-1]), lines, 0))

    lines: list[str] = []
    query, dedent = None, 0
    for i, line in enumerate(f_lines):
        line = line.rstrip()
        if line.startswith("#"):
            continue
        if len(line) == 0 or line[0].isspace():
            if not lines:
                stripped = line.lstrip()
                dedent = len(line) - len(stripped)
                lines.append(stripped)
            else:
                s1, s2 = line.lstrip(), line[dedent:]
                lines.append(s2 if len(s2) > len(s1) else s1)
        else:
            if not line.endswith(":"):
                raise Exception(f'Cannot parse line {i + 1} of pattern file "{fname}"')
            if query:
                add_pattern(query, lines)
            query = line
            lines = []
    if query:
        add_pattern(query, lines)
    return patterns


# --------------------------------------------------------------------------- #
#  Renderer                                                                    #
# --------------------------------------------------------------------------- #

# A run of dotted identifiers, e.g. ``endstone._python.command.CommandSender``.
# group(1) = leading "a.b.c." (with trailing dot), group(2) = final "Name".
_SEP_BEFORE = r"(?<![\\B\.])"
_SEP_AFTER = r"(?![\\B\.])"
_IDENT = r"[^\d\W]\w*"
_ID_SEQ = re.compile(_SEP_BEFORE + "((?:" + _IDENT + r"\.)+)(" + _IDENT + r")\b" + _SEP_AFTER)


class StubGen:
    """Render one griffe ``Module`` to ``.pyi`` text.

    ``top`` is the package root (used for import resolution / same-package
    detection); ``mod`` is the specific module whose body we render.
    """

    def __init__(
        self,
        top: Module,
        mod: Module,
        *,
        include_docstrings: bool = True,
        include_private: bool = False,
        include_values: bool = True,
        patterns: list[ReplacePattern] | None = None,
        rename: tuple[str, str] | None = None,
        aliases: dict[str, str] | None = None,
        quiet: bool = True,
    ) -> None:
        self.top = top
        self.mod = mod
        self.include_docstrings = include_docstrings
        self.include_private = include_private
        self.include_values = include_values
        self.patterns = patterns or []
        self.rename = rename
        self.aliases = aliases or {}
        self.quiet = quiet

        self.depth = 0
        self._output = io.StringIO()

        # Fully-qualified path of the object currently being rendered; a nested
        # type is emitted by its short name within its enclosing scope (see
        # _process_name). Mirrors nanobind's StubGen.prefix.
        self.prefix = mod.path

        # Imports discovered while rendering the body.
        # ``import <module>`` entries (module -> optional alias):
        self._import_modules: dict[str, str | None] = {}
        # ``from <module> import <name>`` entries (module -> {name: bound name}):
        self._import_from: dict[str, dict[str, str]] = {}
        # Names already bound by an import, so two modules cannot claim the same one.
        self._bound: dict[str, str] = {}

        self._local_names = _local_class_names(mod)

        # Names declared via the pattern file's \export directive.
        self._extra_exports: list[str] = []

    # ---- output primitives ----

    def write(self, s: str) -> None:
        self._output.write(s)

    def write_ln(self, line: str = "") -> None:
        if line and not line.isspace():
            self._output.write("    " * self.depth + line)
        self._output.write("\n")

    def _replace_tail(self, num_chars: int, replacement: str) -> None:
        self._output.seek(self._output.tell() - num_chars)
        self._output.truncate()
        self._output.write(replacement)

    def format_docstr(self, docstr: str, depth: int) -> str:
        # Always the expanded block form (ruff stub style). cleandoc, not dedent:
        # an overload's text arrives with its first line already flush and the
        # rest still indented, which dedent leaves alone.
        docstr = inspect.cleandoc(docstr)
        raw = ""
        if "''" in docstr or "\\" in docstr:
            docstr = docstr.replace("''", "\\'\\'")
            raw = "r"
        return textwrap.indent(f'{raw}"""\n{docstr}\n"""\n', "    " * depth)

    def put_docstr(self, docstr: str) -> None:
        self.write(self.format_docstr(docstr, self.depth))

    # ---- imports / name resolution ----

    def import_object(self, module: str, name: str | None = None, as_name: str | None = None) -> str:
        """Register an import, avoiding collisions with local names (mirrors nanobind).

        With ``name`` None the whole module is imported; otherwise ``name`` is
        imported from ``module``, underscore-prefixed if it would shadow a member
        defined in this module. Returns the name actually bound in the stub.
        """
        if name is None:
            self._import_modules.setdefault(module, as_name)
            return as_name or module
        binds = self._import_from.setdefault(module, {})
        if name in binds:
            return binds[name]
        bound = as_name if as_name else name
        if not as_name:
            while bound in self.mod.members or self._bound.get(bound, module) != module:
                bound = "_" + bound
        binds[name] = bound
        self._bound[bound] = module
        return bound

    def check_party(self, module: str) -> int:
        """0 = stdlib, 1 = third-party, 2 = first-party (same top package)."""
        head = module.split(".", 1)[0]
        if head == self.top.path.split(".", 1)[0]:
            return 2
        if head in sys.stdlib_module_names:
            return 0
        return 1

    def _split_internal(self, full: str) -> tuple[str, str]:
        """Split an in-package dotted name into (module_path, qualname)."""
        prefix = self.top.path
        if full == prefix:
            return prefix, ""
        rest = full[len(prefix) + 1 :]
        segs = rest.split(".")
        node, mod_path, i = self.top, prefix, 0
        while i < len(segs) and segs[i] in node.modules:
            node = node.modules[segs[i]]
            mod_path = node.path
            i += 1
        return mod_path, ".".join(segs[i:])

    def _process_name(self, m: "re.Match[str]") -> str:
        full = m.group(0)
        # Resolved in turn, so the alias brings its own import and the name it
        # replaces brings none.
        if (alias := self.aliases.get(full)) is not None:
            return self.simplify(alias) if alias != full else alias
        mod_name = m.group(1)[:-1]
        cls_name = m.group(2)

        if mod_name == "builtins":
            return "None" if cls_name == "NoneType" else cls_name

        prefix = self.top.path
        if full == prefix or full.startswith(prefix + "."):
            mod_path, qual = self._split_internal(full)
            if mod_path == self.mod.path:
                # Strip the enclosing-class prefix so a nested type renders by
                # its short name within that class's body (mirrors nanobind).
                scope = self.prefix[len(self.mod.path) + 1 :]
                enclosing = scope.rpartition(".")[0]
                if enclosing and qual.startswith(enclosing + "."):
                    qual = qual[len(enclosing) + 1 :]
                return qual or full
            # In-package names read as short names; everything else stays qualified.
            head, _, nested = qual.partition(".")
            bound = self.import_object(mod_path, head)
            return f"{bound}.{nested}" if nested else bound

        # A same-module class/enum referenced by short name (pybind11 drops the
        # package prefix, e.g. RenderType.INTEGER) is already in scope; keep bare.
        if full.split(".", 1)[0] in self._local_names:
            return full

        self.import_object(mod_name)
        return full

    def simplify(self, s: str) -> str:
        """Rewrite dotted names in a rendered type string and register imports."""
        if self.rename is not None:
            s = s.replace(self.rename[0], self.rename[1])
        return _ID_SEQ.sub(self._process_name, s)

    def type_str(self, annotation) -> str:
        if annotation is None:
            return "None"
        return self.simplify(str(annotation))

    def any_type(self) -> str:
        self.import_object("typing")
        return "typing.Any"

    def _render_value(self, value, owner=None) -> str:
        # Rendered verbatim; simplify() must not touch a literal (a dotted
        # substring like 'minecraft:player.hunger' would import a bogus module).
        s = str(value)
        # pybind11 native-enum value, e.g. <GameMode.ADVENTURE: 2>.
        if m := _ENUM_RE.fullmatch(s):
            qualified, number = m.group("enum"), m.group("value")
            enum_class = qualified.rsplit(".", 1)[0]
            # Member definition -> its int value; a reference -> the enum name.
            if (
                owner is not None
                and owner.parent is not None
                and owner.parent.kind == Kind.CLASS
                and owner.parent.name == enum_class
            ):
                return number
            return self.simplify(qualified)
        # An opaque repr, or a value suppressed via --exclude-values -- but keep
        # enum members, which nanobind never abbreviates.
        if (not self.include_values and not _is_enum_member(owner)) or "<" in s or ">" in s or "\n" in s:
            return "..."
        try:
            ast.parse(s, mode="eval")
        except SyntaxError:
            # A __str__ that is not a Python expression, e.g. an Identifier's
            # "namespace:key"; the stub shows it as the string it prints as.
            return repr(s)
        return s

    # ---- dispatch ----

    def apply_pattern(self, query: str, obj) -> ReplacePattern | None:
        """Apply the first pattern-file entry whose query matches (mirrors nanobind)."""
        match = None
        pattern = None
        for pattern in self.patterns:
            match = pattern.query.search(query)
            if match:
                break
        if not match or not pattern:
            return None
        for line in pattern.lines:
            ls = line.strip()
            if ls == "\\doc":
                doc = None
                sigs = getattr(obj, "_pybind11_signature_", None)
                if sigs:
                    for s in sigs:
                        doc = s[1]
                        if doc:
                            break
                elif obj is not None and getattr(obj, "docstring", None) is not None:
                    doc = obj.docstring.value
                self.depth += 1
                if doc and self.include_docstrings:
                    self.put_docstr(doc)
                else:
                    self.write_ln("...")
                self.depth -= 1
                continue
            elif ls.startswith("\\from "):
                items = ls[5:].split(" import ")
                if len(items) != 2:
                    raise RuntimeError(f"Could not parse import declaration {ls}")
                for item in items[1].strip("()").split(","):
                    item_list = item.split(" as ")
                    import_module, import_name = items[0].strip(), item_list[0].strip()
                    import_as = item_list[1].strip() if len(item_list) > 1 else None
                    self.import_object(import_module, import_name, import_as)
                continue
            elif ls.startswith("\\import "):
                for mod in ls[7:].split(","):
                    items = mod.split(" as ")
                    if len(items) == 1:
                        modname, as_name = items[0].strip(), None
                    elif len(items) == 2:
                        modname, as_name = [i.strip() for i in items]
                    else:
                        raise RuntimeError(f"Could not parse import declaration {mod}")
                    self.import_object(modname, None, as_name=as_name)
                continue
            elif ls.startswith("\\export "):
                self._extra_exports.extend(n.strip() for n in ls[7:].split(","))
                continue
            if "\\value" in line and obj is not None:
                line = line.replace("\\value", self._render_value(obj.value, obj))
            groups = match.groups()
            for i in reversed(range(len(groups))):
                line = line.replace(f"\\{i + 1}", groups[i])
            for k, v in match.groupdict().items():
                line = line.replace(f"\\{k}", v)
            self.write_ln(line)
        pattern.matches += 1
        return pattern

    def pattern_text(self, query: str) -> str | None:
        """Apply a pattern for its text rather than emitting it; imports still register."""
        saved_output, saved_depth = self._output, self.depth
        self._output, self.depth = io.StringIO(), 0
        try:
            matched = self.apply_pattern(query, None)
            text = self._output.getvalue().strip()
        finally:
            self._output, self.depth = saved_output, saved_depth
        return text if matched else None

    def put(self, obj) -> bool:
        """Emit ``obj``; returns whether anything was written."""
        if isinstance(obj, Alias):
            return False
        name = obj.name
        if name in SKIP_LIST:
            return False
        if name.startswith("__pybind11"):
            return False
        if is_private(name) and not self.include_private:
            return False
        # Drop an undocumented __repr__/__str__; object's says the same.
        if name in _REDUNDANT_DUNDERS and getattr(obj, "_pybind11_signature_", None) == [("def (self) -> str", None)]:
            return False
        # Drop pybind11's default __init__ (carries object.__init__'s docstring).
        if (
            obj.kind == Kind.FUNCTION
            and name == "__init__"
            and obj.docstring is not None
            and obj.docstring.value == object.__init__.__doc__
        ):
            return False

        old_prefix = self.prefix
        self.prefix = self.prefix + (("." + name) if name else "")
        before = self._output.tell()
        try:
            if self.apply_pattern(self.prefix, obj):
                return self._output.tell() > before
            kind = obj.kind
            if kind == Kind.CLASS:
                self.put_type(obj)
            elif kind == Kind.FUNCTION:
                self.put_function(obj)
            elif kind == Kind.ATTRIBUTE:
                if "property" in obj.labels:
                    self.put_property(obj)
                else:
                    self.put_value(obj)
            # Submodules are emitted as separate files by the driver.
        finally:
            self.prefix = old_prefix
        return self._output.tell() > before

    # ---- class ----

    def put_type(self, cls: Class) -> None:
        # Skip pybind11 implementation-detail bases.
        bases = [
            self.simplify(str(b)) for b in cls.bases if str(b) not in {"object", "pybind11_builtins.pybind11_object"}
        ]
        if (override := self.pattern_text(self.prefix + ".__bases__")) is not None:
            bases = [override]
        header = "class " + cls.name
        if bases:
            header += "(" + ", ".join(bases) + ")"
        self.write_ln(header + ":")

        self.depth += 1
        body_start = self._output.tell()
        if cls.docstring and self.include_docstrings and cls.docstring.value:
            self.put_docstr(cls.docstring.value)
        self.apply_pattern(self.prefix + ".__prefix__", None)
        for member in cls.members.values():
            self.put(member)
        self.apply_pattern(self.prefix + ".__suffix__", None)
        if self._output.tell() == body_start:
            self.write_ln("...")
        self.depth -= 1
        self.write("\n")

    # ---- functions ----

    def put_function(self, func: Function) -> None:
        if getattr(func, "_pybind11_signature_", None):
            self.put_py_func(func)
        else:
            self._put_griffe_func(func)

    def put_py_func(self, func: Function) -> None:
        sigs = func._pybind11_signature_
        if len(sigs) == 1:
            self.put_py_overload(func, sigs[0])
            return
        # Overloaded: keep a shared docstring only on its last occurrence.
        last_idx: dict = {}
        for i, s in enumerate(sigs):
            if s[1] is not None:
                last_idx[s[1]] = i
        for i, s in enumerate(sigs):
            if s[1] is not None and last_idx[s[1]] != i:
                s = (s[0], None)
            self.import_object("typing")
            self.write_ln("@typing.overload")
            self.put_py_overload(func, s)

    def put_py_overload(self, func: Function, sig: tuple) -> None:
        sig_str, docstr = sig[0], sig[1]
        if sig_str.startswith("def (") and func.name:
            sig_str = "def " + func.name + sig_str[4:]
        paren = sig_str.find("(")
        sig_str = sig_str[:paren] + self.simplify(sig_str[paren:])
        if "staticmethod" in func.labels:
            self.write_ln("@staticmethod")
        if "classmethod" in func.labels:
            self.write_ln("@classmethod")
        for line in sig_str.split("\n"):
            self.write_ln(line)
        if not docstr or not self.include_docstrings:
            self._replace_tail(1, ": ...\n")
        else:
            self._replace_tail(1, ":\n")
            self.depth += 1
            self.put_docstr(docstr)
            self.depth -= 1
        self.write("\n")

    def _put_griffe_func(self, func: Function) -> None:
        # Non-pybind functions (e.g. inherited buffer-protocol slots) whose
        # signature griffe recovered via inspect.signature.
        if "staticmethod" in func.labels:
            self.write_ln("@staticmethod")
        if "classmethod" in func.labels:
            self.write_ln("@classmethod")
        sig = self._signature_str(func)
        docstr = func.docstring.value if func.docstring else None
        if docstr and self.include_docstrings:
            self.write_ln(f"def {func.name}{sig}:")
            self.depth += 1
            self.put_docstr(docstr)
            self.depth -= 1
        else:
            self.write_ln(f"def {func.name}{sig}: ...")
        self.write("\n")

    def _signature_str(self, func: Function) -> str:
        params = list(func.parameters)
        any_type = self.any_type()
        inner = self._params_str(params) if params else f"*args: {any_type}, **kwargs: {any_type}"
        if func.returns is not None:
            ret = self.type_str(func.returns)
        else:
            # An unannotated __init__ returns None, as every type checker assumes.
            ret = "None" if func.name == "__init__" else self.any_type()
        return f"({inner}) -> {ret}"

    def _params_str(self, params) -> str:
        parts: list[str] = []
        prev = None
        for p in params:
            if prev == ParameterKind.positional_only and p.kind != ParameterKind.positional_only:
                parts.append("/")
            if p.kind == ParameterKind.keyword_only and prev != ParameterKind.keyword_only:
                parts.append("*")
            s = ""
            if p.kind == ParameterKind.var_positional:
                s += "*"
            elif p.kind == ParameterKind.var_keyword:
                s += "**"
            s += p.name
            if p.annotation is not None:
                s += ": " + self.type_str(p.annotation)
            elif p.name != "self":
                s += ": " + self.any_type()
            if p.default is not None:
                s += (" = " if p.annotation is not None else "=") + self._render_value(p.default)
            parts.append(s)
            prev = p.kind
        if prev == ParameterKind.positional_only:
            parts.append("/")
        return ", ".join(parts)

    # ---- properties ----

    def put_property(self, attr: Attribute) -> None:
        ann = getattr(attr, "_pybind11_annotation_", None) or attr.annotation
        ret = self.type_str(ann) if ann is not None else self.any_type()
        self.write_ln("@property")
        docstr = attr.docstring.value if attr.docstring else None
        if docstr and self.include_docstrings:
            self.write_ln(f"def {attr.name}(self) -> {ret}:")
            self.depth += 1
            self.put_docstr(docstr)
            self.depth -= 1
        else:
            self.write_ln(f"def {attr.name}(self) -> {ret}: ...")
        self.write("\n")

        setter = getattr(attr, "setter", None)
        if setter is not None:
            self.write_ln(f"@{attr.name}.setter")
            self.put_function(setter)

        deleter = getattr(attr, "deleter", None)
        if deleter is not None:
            self.write_ln(f"@{attr.name}.deleter")
            self.put_function(deleter)

    # ---- values / attributes ----

    def put_value(self, attr: Attribute) -> None:
        line = attr.name
        value = self._render_value(attr.value, attr) if attr.value is not None else "..."
        if attr.annotation is not None:
            line += ": " + self.type_str(attr.annotation)
        elif not _is_enum_member(attr) and (ann := _literal_type(value)):
            # An enum member is exempt; any other bare value needs one.
            line += ": " + ann
        self.write_ln(line + " = " + value)
        # Values pack together (an enum body reads better dense); only a
        # documented one gets a separating blank line.
        if attr.docstring and self.include_docstrings and attr.docstring.value:
            self.put_docstr(attr.docstring.value)
            self.write("\n")

    # ---- assembly ----

    def render(self) -> None:
        emitted: list[str] = []
        self.apply_pattern(self.prefix + ".__prefix__", None)
        for member in self.mod.members.values():
            if isinstance(member, Alias) or member.kind == Kind.MODULE:
                continue
            if self.put(member):
                emitted.append(member.name)
        self.apply_pattern(self.prefix + ".__suffix__", None)
        # A pybind11 module has no __all__, so griffe leaves exports unset;
        # derive it from what was actually emitted (pattern-dropped members
        # therefore drop out of __all__ too).
        if self.mod.exports is None:
            self.mod.exports = sorted(set(emitted) | set(self.mod.modules) | set(self._extra_exports))

    def _imports_block(self) -> str:
        groups: list[list[str]] = [[], [], []]
        for module, alias in self._import_modules.items():
            stmt = f"import {module} as {alias}" if alias else f"import {module}"
            groups[self.check_party(module)].append(stmt)
        for module, binds in self._import_from.items():
            names = [name if bound == name else f"{name} as {bound}" for name, bound in binds.items()]
            joined = ", ".join(sorted(names))
            groups[self.check_party(module)].append(f"from {module} import {joined}")

        chunks = []
        for group in groups:
            if group:
                # bare "import X" before "from X import ..." within a group
                bare = sorted(x for x in group if x.startswith("import "))
                frm = sorted(x for x in group if x.startswith("from "))
                chunks.append("\n".join(bare + frm))
        return "\n\n".join(chunks)

    def _submodule_block(self) -> str:
        # Plain names, not the redundant "x as x" re-export form: __all__ already
        # marks them exported, and isort splits aliased imports one per statement.
        subs = [name for name in self.mod.modules]
        if not subs:
            return ""
        if len(subs) == 1:
            return f"from . import {subs[0]}"
        lines = ",\n".join(f"    {s}" for s in subs)
        return "from . import (\n" + lines + "\n)"

    def get(self) -> str:
        body = self._output.getvalue()

        out = ""
        if self.mod.docstring and self.include_docstrings and self.mod.docstring.value:
            out += self.format_docstr(self.mod.docstring.value, 0) + "\n"

        imports = self._imports_block()
        if imports:
            out += imports + "\n\n"

        submodules = self._submodule_block()
        if submodules:
            out += submodules + "\n\n"

        if self.mod.exports:
            out += "__all__ = [\n"
            for name in self.mod.exports:
                out += f'    "{name}",\n'
            out += "]\n\n"

        out += body
        return out.rstrip() + "\n"


# --------------------------------------------------------------------------- #
#  Driver / CLI                                                                #
# --------------------------------------------------------------------------- #


def _iter_modules(mod: Module):
    yield mod
    for child in mod.modules.values():
        yield from _iter_modules(child)


def _target_file(mod: Module, top: Module, out_dir: Path) -> Path:
    top_parent = top.path.rsplit(".", 1)[0] if "." in top.path else ""
    rel = mod.path[len(top_parent) + 1 :] if top_parent else mod.path
    segs = rel.split(".")
    if mod.modules:  # package -> its own directory + __init__.pyi
        return out_dir.joinpath(*segs) / "__init__.pyi"
    return out_dir.joinpath(*segs[:-1]) / (segs[-1] + ".pyi")


def render_module(top: Module, mod: Module, opt: argparse.Namespace, patterns: list[ReplacePattern]) -> str:
    sg = StubGen(
        top,
        mod,
        include_docstrings=opt.include_docstrings,
        include_private=opt.include_private,
        include_values=not opt.exclude_values,
        patterns=patterns,
        quiet=opt.quiet,
    )
    sg.render()
    return sg.get()


def parse_options(args: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        prog="python -m endstone.python.stubgen",
        description="Generate stubs for pybind11 extensions (griffe-backed).",
    )
    parser.add_argument(
        "-o",
        "--output-file",
        dest="output_file",
        default=None,
        metavar="FILE",
        help="write the stub to this file (single module)",
    )
    parser.add_argument(
        "-O", "--output-dir", dest="output_dir", default=None, metavar="PATH", help="write stubs under this directory"
    )
    parser.add_argument(
        "-m",
        "--module",
        action="append",
        dest="modules",
        default=[],
        metavar="MODULE",
        help="module to generate a stub for (repeatable)",
    )
    parser.add_argument(
        "-r", "--recursive", action="store_true", dest="recursive", default=False, help="recurse into submodules"
    )
    parser.add_argument(
        "-p", "--pattern-file", dest="pattern_file", default=None, metavar="FILE", help="apply the given pattern file"
    )
    parser.add_argument(
        "-P",
        "--include-private",
        action="store_true",
        dest="include_private",
        default=False,
        help="include single-underscore private members",
    )
    parser.add_argument(
        "-D",
        "--exclude-docstrings",
        action="store_false",
        dest="include_docstrings",
        default=True,
        help="exclude docstrings from the generated stub",
    )
    parser.add_argument(
        "-M",
        "--marker-file",
        action="append",
        dest="marker_file",
        default=[],
        metavar="FILE",
        help="also emit a marker file (e.g. py.typed)",
    )
    parser.add_argument(
        "-i",
        "--import",
        action="append",
        dest="imports",
        default=[],
        metavar="PATH",
        help="add the directory to the Python import path (repeatable)",
    )
    parser.add_argument(
        "--exclude-values",
        action="store_true",
        dest="exclude_values",
        default=False,
        help="force the use of ... for values",
    )
    parser.add_argument("-q", "--quiet", action="store_true", default=False, help="suppress progress output")

    opt = parser.parse_args(args)
    if not opt.modules:
        parser.error("At least one module must be specified.")
    if len(opt.modules) > 1 and opt.output_file:
        parser.error("-o can only be used with a single module.")
    if opt.recursive and opt.output_file:
        parser.error("-o is not compatible with recursive stub generation (-r).")
    return opt


def main(args: list[str] | None = None) -> None:
    opt = parse_options(sys.argv[1:] if args is None else args)

    for path in opt.imports:
        sys.path.insert(0, path)

    patterns = load_pattern_file(opt.pattern_file) if opt.pattern_file else []

    for i, module_name in enumerate(opt.modules):
        if not opt.quiet:
            if i:
                print()
            print(f'Module "{module_name}" ..')
            print("  - loading ..")
        top = load(module_name)

        modules = list(_iter_modules(top)) if opt.recursive else [top]

        if not opt.quiet:
            print("  - rendering ..")
        for mod in modules:
            text = render_module(top, mod, opt, patterns)

            if opt.output_file:
                file = Path(opt.output_file)
            elif opt.output_dir:
                file = _target_file(mod, top, Path(opt.output_dir))
            else:
                file = _target_file(mod, top, Path.cwd())

            file.parent.mkdir(parents=True, exist_ok=True)
            if not opt.quiet:
                print(f'  - writing "{file}" ..')
            file.write_text(text, encoding="utf-8")

    for marker in opt.marker_file:
        path = Path(marker)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.touch()


if __name__ == "__main__":
    main()
