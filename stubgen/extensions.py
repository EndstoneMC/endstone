import ast
import builtins
import enum
import re
import typing
from collections import defaultdict
from typing import Any

from griffe import (
    Alias,
    Attribute,
    Class,
    Docstring,
    Expr,
    ExprAttribute,
    ExprConstant,
    ExprName,
    Extension,
    Function,
    Inspector,
    Module,
    Object,
    ObjectNode,
    Parameter,
    ParameterKind,
    Parameters,
    Visitor,
    get_parameters,
    logger,
    parse_docstring_annotation,
    safe_get_annotation,
    safe_get_expression,
)


class Pybind11SubmoduleSupport(Extension):
    """
    griffe extension to handle submodules of pybind11.

    Normally, griffe skips inspecting a submodule if its `__file__` attribute is set.
    The assumption is that such a submodule corresponds to a real module on disk,
    and therefore it will be inspected recursively by walking the filesystem.

    This assumption breaks for pybind11-based extensions: all submodules share the
    same `__file__`, pointing to the parent binary module. As a result, griffe
    incorrectly treats them as on-disk modules and skips their inspection entirely.

    This extension adjusts that behavior so that submodules defined inside a single
    binary extension (e.g., pybind11 modules) are inspected when their `__file__`
    points to the parent module's binary.
    """

    def on_module_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        mod: Module,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        for child in node.children:
            if child.alias_target_path is None or not child.is_module:
                continue

            if child.alias_target_path != f"{agent.current.path}.{child.name}":
                continue

            if hasattr(child.obj, "__file__") and getattr(child.obj, "__file__") == getattr(node.obj, "__file__"):
                delattr(child.obj, "__file__")


class Pybind11PropertySupport(Extension):
    """
    Griffe extension for adding support for pybind11 properties.

    Behavior:
        - If the inspected attribute is not labeled as a "property", nothing is done.
        - If a setter function (`fset`) exists on the property, a corresponding
          `Function` node is created, attached as the property's setter, labeled
          as "writable", and passed through the standard Griffe extension hooks
          for function instances.
        - If a deleter function (`fdel`) exists on the property, a corresponding
          `Function` node is created, attached as the property's deleter, labeled
          as "deletable", and likewise passed through the extension hooks.

    This allows tools relying on Griffe (e.g., mkdocstrings) to properly
    represent Pybind11-exposed properties with accurate read/write/delete
    semantics in their documentation.
    """

    def on_attribute_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        attr: Attribute,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        if "property" not in attr.labels:
            return

        if fset := getattr(node.obj, "fset", None):
            fset_node = ObjectNode(fset, node.name, node)
            setter = Function(name=node.name, docstring=agent._get_docstring(fset_node), parent=agent.current)
            attr.setter = setter
            attr.labels.add("writable")
            agent.extensions.call("on_instance", node=fset_node, obj=setter, agent=agent)
            agent.extensions.call("on_function_instance", node=fset_node, func=setter, agent=agent)

        if fdel := getattr(node.obj, "fdel", None):
            fdel_node = ObjectNode(fdel, node.name, node)
            deleter = Function(name=node.name, docstring=agent._get_docstring(fdel_node), parent=agent.current)
            attr.deleter = deleter
            attr.labels.add("deletable")
            agent.extensions.call("on_instance", node=fdel_node, obj=deleter, agent=agent)
            agent.extensions.call("on_function_instance", node=fdel_node, func=deleter, agent=agent)


class Pybind11NativeEnumSupport(Extension):
    """
    Griffe extension that normalizes values of `pybind11::native_enum` attributes.

    Pybind11's `native_enum` can expose enumeration members and also "exported"
    enum entries (via `export_values()`), whose values often appear as
    lossy/opaque strings like `"<pkg.Mod.Enum: 1>"` when introspected.
    This extension rewrites such values into stable, documentation-friendly
    representations:

    - If the attribute corresponds to an actual enum member (i.e., the value's
      type is a subclass of its parent enum), the attribute value is replaced by
      an `ExprConstant` containing the *underlying numeric value* (e.g., `"1"`).
    - If the attribute is an exported entry (created via
      `pybind11::native_enum<Enum>::export_values()`), the attribute value is
      replaced by the fully qualified enum name captured from the string
      representation (e.g., `"pkg.Mod.Enum"`).
    """

    def on_attribute_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        attr: Attribute,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        pybind11_enum_pattern = re.compile(r"<(?P<enum>\w+(\.\w+)+): (?P<value>-?\d+)>")
        if isinstance(attr.value, str) and (match := pybind11_enum_pattern.match(attr.value)):
            value = node.obj
            tp = type(value)
            parent = node.parent.obj
            if isinstance(parent, type) and issubclass(tp, parent):
                # This is an entry of an enumeration
                attr.value = ExprConstant(f"{typing.cast(enum.Enum, value).value}")
            else:
                # this is an exported entry (i.e., pybind11::native_enum<Enum>::export_values())
                attr.value = match.group("enum")
                attr.docstring = None


class Pybind11InternalsFilter(Extension):
    def on_members(self, *, node: ast.AST | ObjectNode, obj: Object, agent: Visitor | Inspector, **kwargs: Any) -> None:
        for member in {
            "__annotations__",
            "__builtins__",
            "__cached__",
            "__class__",
            "__dict__",
            "__doc__",
            "__file__",
            "__firstlineno__",
            "__format__",
            "__loader__",
            "__module__",
            "__name__",
            "__package__",
            "__path__",
            "_pybind11_conduit_v1_",
            "__qualname__",
            "__spec__",
            "__static_attributes__",
            "__weakref__",
        }:
            obj.members.pop(member, None)

    def on_attribute_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        attr: Attribute,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        if attr.docstring is not None and attr.docstring.value == type(node.obj).__doc__:
            attr.docstring = None

    def on_class_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        cls: Class,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        cls.bases = [base for base in cls.bases if base != "pybind11_builtins.pybind11_object"]

    def on_class_members(
        self,
        *,
        node: ast.AST | ObjectNode,
        cls: Class,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if "__init__" in cls.members:
            func = cls.members["__init__"]
            if func.docstring is not None and func.docstring.value == object.__init__.__doc__:
                cls.members.pop("__init__")


class Pybind11DocstringParser(Extension):
    def on_attribute_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        attr: Attribute,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        fget = getattr(node.obj, "fget", None)
        if fget is None:
            return

        fget_node = ObjectNode(fget, node.name, node)
        docstring = agent._get_docstring(fget_node)
        if docstring is None:
            return

        top_signature_regex = re.compile(r"^\(.*\)\s*(->\s*(?P<returns>.+))?$")
        match = top_signature_regex.match(docstring.value)
        if match is None:
            return

        returns_str = match.group("returns")
        if returns_str is not None:
            attr.annotation = parse_docstring_annotation(returns_str, attr.docstring)

    def on_function_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        func: Function,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        if not func.docstring:
            return

        doc_lines = func.docstring.lines
        if len(doc_lines) == 0:
            return

        if isinstance(func.parent.get_member(func.name), Attribute):
            # Note: pybind *usually* does not include the function name in getter/setter signatures, e.g.:
            #   (arg0: demo._bindings.enum.ConsoleForegroundColor) -> int
            func_name = ""
        else:
            func_name = func.name

        top_signature_regex = re.compile(rf"^{func_name}\((?P<args>.*)\)\s*(->\s*(?P<returns>.+))?$")
        signature = doc_lines[0]
        if not top_signature_regex.match(signature):
            return

        func.parameters, func.returns = self.parse_signature(
            signature if func_name else func.name + signature, parent=func.parent
        )
        if func.parameters:
            if not func_name and func.parameters[0].name == "arg0":
                func.parameters[0].name = "self"
                func.parameters[0].annotation = None
            elif func_name and func.parameters[0].name == "self":
                func.parameters[0].annotation = None

        if len(doc_lines) < 2 or doc_lines[1] != "Overloaded function.":
            if len(doc_lines) > 1:
                func.docstring.value = "\n".join(doc_lines[1:]).lstrip()
            else:
                func.docstring = None

        else:
            overload_signature_regex = re.compile(
                rf"^(\s*(?P<overload_number>\d+).\s*)" rf"{func_name}\((?P<args>.*)\)\s*->\s*(?P<returns>.+)$"
            )

            doc_start = 0
            overloads = [func]

            for i in range(2, len(doc_lines)):
                match = overload_signature_regex.match(doc_lines[i])
                if match:
                    if match.group("overload_number") != f"{len(overloads)}":
                        continue
                    overloads[-1].docstring = Docstring("\n".join(doc_lines[doc_start:i]).lstrip())
                    doc_start = i + 1

                    overload = Function(name=func.name)
                    signature = f"{func_name}({match.group('args')}) -> {match.group('returns')}"

                    overload.parameters, overload.returns = self.parse_signature(signature, parent=func.parent)
                    if overload.parameters and overload.parameters[0].name == "self":
                        overload.parameters[0].annotation = None

                    overloads.append(overload)

            overloads[-1].docstring = Docstring("\n".join(doc_lines[doc_start:]).lstrip())

            func.overloads = overloads[1:]
            func.docstring = None

    def parse_signature(self, signature: str, *, parent: Module | Class) -> tuple[Parameters, str | Expr | None]:
        try:
            pybind11_enum_pattern = re.compile(r"<(?P<enum>\w+(\.\w+)+): (?P<value>-?\d+)>")
            signature = pybind11_enum_pattern.sub(r"\g<enum>", signature)
            node = compile(
                f"def {signature}:...",
                mode="exec",
                filename="<>",
                flags=ast.PyCF_ONLY_AST,
                optimize=2,
            )
            node = node.body[0]
            parameters = Parameters(
                *[
                    Parameter(
                        name,
                        kind=kind,
                        annotation=safe_get_annotation(annotation, parent=parent, member=node.name),
                        default=None
                        if isinstance(default, str)
                        else safe_get_expression(default, parent=parent, parse_strings=False),
                    )
                    for name, annotation, kind, default in get_parameters(node.args)
                ],
            )
            returns = safe_get_annotation(node.returns, parent=parent, member=node.name)
            return parameters, returns

        except Exception as e:
            logger.warning(e, exc_info=True)
            parameters = Parameters(
                Parameter("args", kind=ParameterKind.var_positional),
                Parameter("kwargs", kind=ParameterKind.var_keyword),
            )

            return parameters, None


class Pybind11ImportFix(Extension):
    def __init__(self):
        self.module: Module | None = None
        self.current: Module | Class | None = None

    def on_module_instance(
        self,
        *,
        node: ast.AST | ObjectNode,
        mod: Module,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        if self.module is not None:
            return

        self.module = mod

    def on_module_members(
        self,
        *,
        node: ast.AST | ObjectNode,
        mod: Module,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if self.module is not mod:
            return

        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        self.current = mod
        self.handle_module(mod)
        self.module = None

    def handle_module(self, mod: Module) -> None:
        self.current = mod
        for keys in list(mod.members.keys()):
            child = mod.members[keys]
            if isinstance(child, Alias):
                continue

            if cb := getattr(self, f"handle_{child.kind.name.lower()}", None):
                cb(child)

        import_list = []
        importfrom_list = defaultdict(list)
        for as_name, name in mod.imports.items():
            if as_name == name:
                import_list.append(name)
            else:
                module, obj = name.rsplit(".", maxsplit=1)
                importfrom_list[module].append(obj)

        imports: dict[str, str] = {}
        for name in sorted(import_list):
            imports[name] = name

        for module in sorted(importfrom_list.keys()):
            objects = importfrom_list[module]
            for obj in sorted(objects):
                imports[obj] = f"{module}.{obj}"

        mod.imports = imports

    def handle_class(self, cls: Class) -> None:
        self.current = cls

        cls.bases = [self._add_import(base) for base in cls.bases]
        for keys in list(cls.members.keys()):
            child = cls.members[keys]
            if isinstance(child, Alias):
                continue

            if cb := getattr(self, f"handle_{child.kind.name.lower()}", None):
                cb(child)

        self.current = self.current.parent

    def handle_function(self, func: Function) -> None:
        func.returns = self._add_import(func.returns)
        for parameter in func.parameters:
            if parameter.annotation is not None:
                parameter.annotation = self._add_import(parameter.annotation)

        if func.overloads:
            self._add_import("typing.overload")
            for overload in func.overloads:
                self.handle_function(overload)

    def handle_attribute(self, attr: Attribute) -> None:
        attr.annotation = self._add_import(attr.annotation)

        if attr.setter:
            self.handle_function(attr.setter)

        if attr.deleter:
            self.handle_function(attr.deleter)

    def _add_import(self, name: Expr | str | None) -> Expr | str | None:
        if not name:
            return name

        if isinstance(name, Expr):
            if isinstance(name, ExprAttribute) or isinstance(name, ExprName):
                name = name.path
            else:
                result = "".join([self._add_import(e) if isinstance(e, Expr) else e for e in name])
                return result

        if hasattr(builtins, name):
            return name

        if name.startswith(self.module.path + "."):
            # internal import
            target = self.module.get_member(name[len(self.module.path) + 1 :])

            if self.current.is_class and target.parent.path == self.current.path:
                return target.name

            if target.module.path == self.current.module.path:
                return target.path[len(self.current.module.path) + 1 :]

            alias_path = name
            alias_name = name.rsplit(".", 1)[-1]
            final_name = alias_name

        else:
            # external import
            alias_path = name.split(".", 1)[0]
            alias_name = alias_path.split(".", 1)[0]
            final_name = name

        if alias_name in self.current.module.imports:
            if self.current.module.imports[alias_name] != alias_path:
                logger.warning(
                    f"Alias {alias_name} already exists with different path: {self.current.module.imports[alias_name]}"
                )

            return final_name

        if alias_name in self.current.module.members:
            logger.warning(f"Member {alias_name} already exists in {self.current.module.path}")
            return final_name

        self.current.module.imports[alias_name] = alias_path
        alias = Alias(alias_name, alias_path)
        self.current.module.set_member(alias_name, alias)
        # agent.extensions.call("on_alias_instance", alias=alias, node=node, agent=agent)

        return final_name


class Pybind11ExportFix(Extension):
    def on_module_members(
        self,
        *,
        node: ast.AST | ObjectNode,
        mod: Module,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        if mod.exports is not None:
            return

        mod.exports = sorted([child.name for child in mod.members.values() if not isinstance(child, Alias)])


class MemberOrderFix(Extension):
    """
    griffe extension to preserve the original definition order of members.

    By default, griffe relies on `inspect.getmembers` when collecting members
    of runtime objects. Unfortunately, `inspect.getmembers` always returns
    members sorted alphabetically, which loses the original order in which
    attributes and methods were defined.

    This extension fixes that behavior by reordering the collected members
    according to the order they appear in the object's `__dict__`. This way,
    griffe preserves the natural definition order of attributes and methods.
    """

    def on_members(
        self,
        *,
        node: ast.AST | ObjectNode,
        obj: Object,
        agent: Visitor | Inspector,
        **kwargs: Any,
    ) -> None:
        if not isinstance(node, ObjectNode) or not isinstance(agent, Inspector):
            return

        members = {}
        for k, v in node.obj.__dict__.items():
            if k in obj.members:
                members[k] = obj.members.pop(k)

        obj.members.update(members)
