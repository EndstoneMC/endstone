---
paths:
  - "src/endstone/python/**"
  - "endstone/**"
---

# Python

Ruff config is `pyproject.toml`.

## Bindings

- Bind enums with `py::native_enum<T>(..., "enum.Enum", doc)` ending in `.export_values().finalize()`.
- Give every `.def` explicit `snake_case` `py::arg("name")` names, with a trailing underscore where the name is a Python keyword, such as `py::arg("namespace_")`.
- Give every new public class and property a binding in `src/endstone/python/*.cpp`, mirroring the C++ getter.
- Bind handle-managed and polymorphic classes with `py::classh<T, Base>` or `py_class<T>` from `endstone_python.h`; bind value types, form builders and enum/registry singletons as plain `py::class_`.
- Derive every trampoline from `py::trampoline_self_life_support`.
- Register a C++ base class before its subclasses; a feature change inserts into shared registration lists and never moves existing entries, because a base moved after its derived class breaks `import endstone`; for mutually referencing classes declare both `py::class_` handles before any `.def`.
- Add every new `ItemMeta` or other polymorphic subclass to `polymorphic_type_hook` in `poly.h`.
- List `ICancellable` in the `py::class_` bases of every `Cancellable<...>` event.
- Add new submodules in `endstone_python.cpp` through `def_submodule` with a one-line description.
- Bind identifier constants with `.def_property_readonly_static("NAME", id(X::Y))`, using `id()` for `Identifier<T>` and `constant()` for anything else.
- Bind every `Registry<T>::Type` through `def_registry_type(py::class_<T>(...))`; a type equal to its id string hashes that string.
- Bind STL containers of handles as opaque with `bind_vector`, `bind_map` or `bind_set` from `endstone_python.h`.
- Bind live recipient sets (`PlayerSet`, `CommandSenderSet`) as `MutableSet`-registered classes whose iteration is a snapshot raising "Set changed size during iteration".

## Lifetime and casters

- Never let a raw `T&`, `T*` or `unique_ptr` cross the binding boundary.
- Cast through the bound type when returning `std::ref(*this)` to Python; deduction picks the unbound type.
- Hand events to the Python executor as `NotNull<Event>`, never `Event *` or `Event &`.
- Never bind a registry entry, event member or handle-managed object with `return_value_policy::reference`; return a handle.
- Never bind containers of borrowed raw pointers (`reference_internal`) where C++ can erase the owner.
- A custom `type_caster` throws the intended exception for a right-typed, wrong-valued argument and returns `false` only for a type mismatch.
- Clear any `std::function` holding a Python callable that captures its wrapper on disconnect.
- Built-in cancellable events satisfy `isinstance(e, endstone.event.Cancellable)` through the metaclass in `endstone/event/__init__.py`; never use `abc.ABC.register`.

## Stubs and package

- `endstone/**/__init__.pyi` and the `lazy_loader.attach` lists in package `__init__.py` files are generated; never edit them by hand or run `scripts/stubgen.py` except to verify a stubgen change.
- The `endstone_stubs` CMake target regenerates them whenever `_python` relinks, and a stub that shows an old docstring means stubgen ran against a stale `_python`; commit the result.
- Pass `-DENDSTONE_GENERATE_STUBS=OFF` in every build tree but one per checkout.
- Put stub content with no pybind counterpart in `src/endstone/python/stubgen.pat`; patterns are `re.search`, so anchor each.
- Bind a numeric default longer than 10 characters as `py::arg_v(name, value, "...")`; ruff's PYI054 turns it into `...` in the stub.
- Annotations in `STAGED_SOURCES` are fully qualified.
- Type a setter that accepts `None` and every `Nullable` return as `X | None`; registry `get()` returns `X | None`.
- Port `src/endstone/python/stubgen.py` changes line for line from nanobind's `stubgen.py`, deviating only where griffe differs from live-object introspection.
- Verify stubgen changes by regenerating against the installed `_python` with `PYTHONSAFEPATH=1` and diffing against the tracked `.pyi`.
- Never use an editable install.
- Keep files LF; CRLF makes ruff report every line on Windows.
- Before binding a property on the native `Plugin`, check no Python-side plugin metadata attribute (`commands`, `permissions`, ...) uses the name; a subclass's class attribute shadows the native property.
- Register every bound type before any signature that names it, not only bases before subclasses; otherwise stubgen fails on the raw C++ name.
- Never give a trampoline an override whose name is bound as a property; `get_override` finds the property and recurses.
- Add every class with a trampoline to the harness recorder's `DISPATCHED`; wrapping its bound method makes `get_override` find the wrapper and recurse.
