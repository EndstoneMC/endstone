import inspect
import types
import typing
from collections.abc import Callable

if typing.TYPE_CHECKING:
    from endstone._python.command import ArgumentType, Commands

_F = typing.TypeVar("_F", bound=Callable[..., None])

_EMPTY = inspect.Parameter.empty


def command(
    func: _F | None = None,
    *,
    name: str | None = None,
    description: str | None = None,
    aliases: list[str] | None = None,
    permission: str | None = None,
) -> _F | Callable[[_F], _F]:
    """Declares a command tree rooted at this method.

    The literal the player types is the method name unless `name` is given. Arguments are the
    method's own parameters, after `self` and the context - see `subcommand` for the details.

    Args:
        name: The word the player types. Defaults to the method name.
        description: A brief description of the command.
        aliases: Alternative names the command may be typed as.
        permission: A permission required to use the command.
    """

    def decorator(f: _F) -> _F:
        _mark(f, name=name, permission=permission)
        f._is_command = True
        f._command_description = description
        f._command_aliases = list(aliases or [])
        return f

    return decorator(func) if func is not None else decorator


def _mark(f, *, name: str | None, permission: str | None) -> None:
    f._command_name = name or f.__name__
    f._command_permission = permission
    f._command_children = []
    f.subcommand = _subcommand_factory(f)


def _subcommand_factory(parent):
    def subcommand(
        func: _F | None = None,
        *,
        name: str | None = None,
        permission: str | None = None,
    ) -> _F | Callable[[_F], _F]:
        """Declares a branch under this command.

        The literal is the method name unless `name` is given. Every parameter after `self` and the
        context becomes an argument: its annotation picks the argument type, and giving it a default
        makes it optional.
        """

        def decorator(f: _F) -> _F:
            _mark(f, name=name, permission=permission)
            f._is_subcommand = True
            parent._command_children.append(f)
            return f

        return decorator(func) if func is not None else decorator

    return subcommand


def _unwrap_optional(annotation):
    """Reduces `X | None` to `X`, leaving anything else alone."""
    origin = typing.get_origin(annotation)
    if origin is typing.Union or origin is types.UnionType:
        args = [a for a in typing.get_args(annotation) if a is not type(None)]
        if len(args) == 1:
            return args[0], True
    return annotation, False


def _spec_for(annotation, parameter_name: str) -> "ArgumentType":
    """Picks the argument type for a parameter annotation."""
    from endstone import Player
    from endstone._python.command import ArgumentType
    from endstone._python.command import arguments as _arguments
    from endstone.actor import Actor

    annotation, _ = _unwrap_optional(annotation)

    if typing.get_origin(annotation) is typing.Annotated:
        for extra in typing.get_args(annotation)[1:]:
            if isinstance(extra, ArgumentType):
                return extra
        annotation, _ = _unwrap_optional(typing.get_args(annotation)[0])

    if isinstance(annotation, ArgumentType):
        return annotation

    if typing.get_origin(annotation) is list:
        args = typing.get_args(annotation)
        item = args[0] if args else None
        if item is Player:
            return _arguments.players()
        if item is Actor:
            return _arguments.entities()
        raise TypeError(
            f"Argument '{parameter_name}' is a list of {item!r}, which has no argument type. "
            f"Annotate it with one, for example list[Player]."
        )

    simple = {
        bool: _arguments.boolean,
        int: _arguments.integer,
        float: _arguments.floating,
        str: _arguments.string,
        Player: _arguments.player,
        Actor: _arguments.entity,
    }
    if annotation in simple:
        return simple[annotation]()

    raise TypeError(
        f"Argument '{parameter_name}' is annotated {annotation!r}, which has no argument type. "
        f"Use typing.Annotated to give it one, for example "
        f"Annotated[str, arguments.soft_enum('my_plugin:warps')]."
    )


def _arguments_of(func) -> list[tuple[str, "ArgumentType", bool]]:
    """Returns (name, type, optional) for every argument parameter of a handler."""
    signature = inspect.signature(func)
    try:
        hints = typing.get_type_hints(func, include_extras=True)
    except Exception:  # noqa: BLE001 - a bad annotation should name the handler, not explode here
        hints = {}

    parameters = [p for p in signature.parameters.values() if p.name != "self"]
    if not parameters:
        raise TypeError(f"Handler '{func.__qualname__}' must take a CommandContext.")

    result = []
    for parameter in parameters[1:]:
        annotation = hints.get(parameter.name, parameter.annotation)
        if annotation is _EMPTY:
            raise TypeError(
                f"Argument '{parameter.name}' of '{func.__qualname__}' needs an annotation to give it an argument type."
            )
        _, nullable = _unwrap_optional(annotation)
        optional = parameter.default is not _EMPTY or nullable
        result.append((parameter.name, _spec_for(annotation, parameter.name), optional))
    return result


def _handler_for(bound, names: list[str]):
    def handler(ctx):
        return bound(ctx, **{name: ctx[name] for name in names if name in ctx})

    return handler


def _build(func, holder) -> "Commands":
    """Builds the branch for one decorated method, and every branch below it."""
    from endstone._python.command import Commands

    builder = Commands.literal(func._command_name)
    if func._command_permission:
        builder = builder.permission(func._command_permission)

    arguments = _arguments_of(func)
    names = [name for name, _, _ in arguments]
    bound = getattr(holder, func.__name__)

    mandatory = [i for i, (_, _, optional) in enumerate(arguments) if not optional]
    last_mandatory = mandatory[-1] if mandatory else -1

    # The chain is built from the tail back, so each argument can be handed the branch that
    # follows it. Everything from the last mandatory argument onwards can end the command.
    tail = None
    for index in range(len(arguments) - 1, -1, -1):
        name, spec, _ = arguments[index]
        node = Commands.argument(name, spec)
        if tail is not None:
            node = node.then(tail)
        if index >= last_mandatory:
            node = node.executes(_handler_for(bound, names))
        tail = node

    if tail is not None:
        builder = builder.then(tail)
    if last_mandatory < 0:
        builder = builder.executes(_handler_for(bound, names))

    for child in func._command_children:
        builder = builder.then(_build(child, holder))
    return builder


def build_command_trees(holder: object) -> list["Commands"]:
    """Returns a builder for every command tree declared on an object with `@command`."""
    trees = []
    for attribute in dir(type(holder)):
        func = getattr(type(holder), attribute, None)
        if not callable(func) or not getattr(func, "_is_command", False):
            continue

        builder = _build(func, holder)
        if func._command_description:
            builder = builder.description(func._command_description)
        if func._command_aliases:
            builder = builder.aliases(func._command_aliases)
        trees.append(builder)
    return trees
