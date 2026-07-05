#!/usr/bin/env python3
# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "griffe",
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
import io
import re
import sys
import textwrap
from dataclasses import dataclass
from pathlib import Path
from re import Pattern
from typing import List, Optional

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


def _clean_sig_args(name: Optional[str], args: str) -> str:
    args = _ENUM_RE.sub(r"\g<enum>", args)
    if name is None:
        return re.sub(r"^arg0:\s*[\w.]+", "self", args)
    return re.sub(r"^self:\s*[\w.]+", "self", args)


def _valid_sig(sig: str) -> bool:
    # pybind11 docstrings can carry non-Python types (e.g. C++ "endstone::Foo").
    try:
        ast.parse(f"def _{sig[4:]}: ...")
        return True
    except SyntaxError:
        return False


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


def _extract_signatures(docstring) -> Optional[List[tuple]]:
    """Parse pybind11 signature(s) from a docstring, mirroring nanobind's __nb_signature__.

    Returns a list of (sig, doc, defaults) where sig is "def (args) -> ret".
    pybind11 inlines defaults into the text, so defaults is always None.
    """
    lines = docstring.lines
    if not lines:
        return None
    head = _PYBIND_SIG_RE.match(lines[0])
    if head is None:
        return None
    if len(lines) >= 2 and lines[1].strip() == "Overloaded function.":
        entries: List[tuple] = []
        doc_start = 2
        for i in range(2, len(lines)):
            m = _PYBIND_SIG_RE.match(lines[i])
            if m and m.group("ret") and m.group("num") == str(len(entries) + 1):
                if entries:
                    entries[-1] = (entries[-1][0], "\n".join(lines[doc_start:i]).strip() or None, None)
                sig = f"def ({_clean_sig_args(m.group('name'), m.group('args'))}) -> {m.group('ret').strip()}"
                if not _valid_sig(sig):
                    raise ValueError(f"unparseable signature {sig[4:]!r}")
                entries.append((sig, None, None))
                doc_start = i + 1
        if not entries:
            return None
        entries[-1] = (entries[-1][0], "\n".join(lines[doc_start:]).strip() or None, None)
        return entries
    if head.group("ret") is None:
        return None
    sig = f"def ({_clean_sig_args(head.group('name'), head.group('args'))}) -> {head.group('ret').strip()}"
    if not _valid_sig(sig):
        raise ValueError(f"unparseable signature {sig[4:]!r}")
    return [(sig, "\n".join(lines[1:]).strip() or None, None)]


class Pybind11Support(Extension):
    """Umbrella griffe extension for pybind11 quirks"""

    def __init__(self) -> None:
        super().__init__()
        self.errors: List[str] = []

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
                attr.annotation = m.group("ret").strip()
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
    lines: List[str]
    matches: int


def load_pattern_file(fname: str) -> List[ReplacePattern]:
    """Load a pattern file; see nanobind's stubgen docs for the syntax."""
    with open(fname, "r", encoding="utf-8") as f:
        f_lines = f.readlines()

    patterns: List[ReplacePattern] = []

    def add_pattern(query: str, lines: List[str]) -> None:
        while lines and (lines[-1].isspace() or len(lines[-1]) == 0):
            lines.pop()
        lines.append("")
        if all((p.isspace() or len(p) == 0 for p in lines)):
            lines = []
        patterns.append(ReplacePattern(re.compile(query[:-1]), lines, 0))

    lines: List[str] = []
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
        patterns: Optional[List[ReplacePattern]] = None,
        quiet: bool = True,
    ) -> None:
        self.top = top
        self.mod = mod
        self.include_docstrings = include_docstrings
        self.include_private = include_private
        self.patterns = patterns or []
        self.quiet = quiet

        self.depth = 0
        self._output = io.StringIO()

        # Fully-qualified path of the object currently being rendered; a nested
        # type is emitted by its short name within its enclosing scope (see
        # _process_name). Mirrors nanobind's StubGen.prefix.
        self.prefix = mod.path

        # Imports discovered while rendering the body.
        # ``import <module>`` entries:
        self._import_modules: set[str] = set()
        # ``from <module> import <name>`` entries:
        self._import_from: dict[str, set[str]] = {}

        self._local_names = _local_class_names(mod)

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
        # Always the expanded block form (ruff stub style).
        docstr = textwrap.dedent(docstr).strip()
        raw = ""
        if "''" in docstr or "\\" in docstr:
            docstr = docstr.replace("''", "\\'\\'")
            raw = "r"
        return textwrap.indent(f'{raw}"""\n{docstr}\n"""\n', "    " * depth)

    def put_docstr(self, docstr: str) -> None:
        self.write(self.format_docstr(docstr, self.depth))

    # ---- imports / name resolution ----

    def _need_import_module(self, module: str) -> None:
        self._import_modules.add(module)

    def _need_from(self, module: str, name: str) -> None:
        self._import_from.setdefault(module, set()).add(name)

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
            self._need_import_module(mod_path)
            return full

        if mod_name in ("typing", "typing_extensions", "collections.abc"):
            self._need_from(mod_name, cls_name)
            return cls_name

        # A same-module class/enum referenced by short name (pybind11 drops the
        # package prefix, e.g. RenderType.INTEGER) is already in scope; keep bare.
        if full.split(".", 1)[0] in self._local_names:
            return full

        self._need_import_module(mod_name)
        return full

    def simplify(self, s: str) -> str:
        """Rewrite dotted names in a rendered type string and register imports."""
        return _ID_SEQ.sub(self._process_name, s)

    def type_str(self, annotation) -> str:
        if annotation is None:
            return "None"
        return self.simplify(str(annotation))

    def any_type(self) -> str:
        self._need_from("typing", "Any")
        return "Any"

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
        # Any other opaque repr -> placeholder.
        if "<" in s or ">" in s or "\n" in s:
            return "..."
        return s

    # ---- dispatch ----

    def _is_private(self, name: str) -> bool:
        return len(name) > 2 and ((name[0] == "_" and name[1] != "_") or (name[-1] == "_" and name[-2] != "_"))

    def put(self, obj) -> None:
        if isinstance(obj, Alias):
            return
        name = obj.name
        if name in SKIP_LIST:
            return
        if self._is_private(name) and not self.include_private:
            return
        # Drop pybind11's default __init__ (carries object.__init__'s docstring).
        if (
            obj.kind == Kind.FUNCTION
            and name == "__init__"
            and obj.docstring is not None
            and obj.docstring.value == object.__init__.__doc__
        ):
            return

        old_prefix = self.prefix
        self.prefix = self.prefix + (("." + name) if name else "")
        try:
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

    # ---- class ----

    def put_type(self, cls: Class) -> None:
        # Skip pybind11 implementation-detail bases.
        bases = [
            self.simplify(str(b))
            for b in cls.bases
            if str(b) not in {"object", "pybind11_builtins.pybind11_object"}
        ]
        header = "class " + cls.name
        if bases:
            header += "(" + ", ".join(bases) + ")"
        self.write_ln(header + ":")

        self.depth += 1
        body_start = self._output.tell()
        if cls.docstring and self.include_docstrings and cls.docstring.value:
            self.put_docstr(cls.docstring.value)
        for member in cls.members.values():
            self.put(member)
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
                s = (s[0], None, s[2])
            self._need_from("typing", "overload")
            self.write_ln("@overload")
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
        inner = self._params_str(params) if params else "*args, **kwargs"
        ret = self.type_str(func.returns) if func.returns is not None else self.any_type()
        return f"({inner}) -> {ret}"

    def _params_str(self, params) -> str:
        parts: List[str] = []
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
            if p.default is not None:
                s += (" = " if p.annotation is not None else "=") + self._render_value(p.default)
            parts.append(s)
            prev = p.kind
        if prev == ParameterKind.positional_only:
            parts.append("/")
        return ", ".join(parts)

    # ---- properties ----

    def put_property(self, attr: Attribute) -> None:
        ret = self.type_str(attr.annotation) if attr.annotation is not None else self.any_type()
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
        if attr.annotation is not None:
            line += ": " + self.type_str(attr.annotation)
        if attr.value is not None:
            line += " = " + self._render_value(attr.value, attr)
        else:
            line += " = ..."
        self.write_ln(line)
        if attr.docstring and self.include_docstrings and attr.docstring.value:
            self.put_docstr(attr.docstring.value)
        self.write("\n")

    # ---- assembly ----

    def render(self) -> None:
        for member in self.mod.members.values():
            if isinstance(member, Alias) or member.kind == Kind.MODULE:
                continue
            self.put(member)

    def _imports_block(self) -> str:
        groups: List[List[str]] = [[], [], []]
        for module in self._import_modules:
            groups[self.check_party(module)].append(f"import {module}")
        for module, names in self._import_from.items():
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
        subs = [name for name in self.mod.modules]
        if not subs:
            return ""
        if len(subs) == 1:
            return f"from . import {subs[0]} as {subs[0]}"
        lines = ",\n".join(f"    {s} as {s}" for s in subs)
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


def render_module(top: Module, mod: Module, opt: argparse.Namespace, patterns: List[ReplacePattern]) -> str:
    sg = StubGen(
        top,
        mod,
        include_docstrings=opt.include_docstrings,
        include_private=opt.include_private,
        patterns=patterns,
        quiet=opt.quiet,
    )
    sg.render()
    return sg.get()


def parse_options(args: List[str]) -> argparse.Namespace:
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
    parser.add_argument("-q", "--quiet", action="store_true", default=False, help="suppress progress output")

    opt = parser.parse_args(args)
    if not opt.modules:
        parser.error("At least one module must be specified.")
    if len(opt.modules) > 1 and opt.output_file:
        parser.error("-o can only be used with a single module.")
    if opt.recursive and opt.output_file:
        parser.error("-o is not compatible with recursive stub generation (-r).")
    return opt


def main(args: Optional[List[str]] = None) -> None:
    opt = parse_options(sys.argv[1:] if args is None else args)

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
