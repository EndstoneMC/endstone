import re
from pathlib import Path

from pybind11_stubgen import run, to_output_and_subdir
from pybind11_stubgen.parser.mixins.error_handlers import (
    LogErrors,
    LoggerData,
    SuggestCxxSignatureFix,
)
from pybind11_stubgen.parser.mixins.filter import (
    FilterClassMembers,
    FilterInvalidIdentifiers,
    FilterPybind11ViewClasses,
    FilterPybindInternals,
    FilterTypingModuleAttributes,
)
from pybind11_stubgen.parser.mixins.fix import (
    FixBuiltinTypes,
    FixCurrentModulePrefixInTypeNames,
    FixMissing__all__Attribute,
    FixMissing__future__AnnotationsImport,
    FixMissingEnumMembersAnnotation,
    FixMissingFixedSizeImport,
    FixMissingImports,
    FixMissingNoneHashFieldAnnotation,
    FixNumpyArrayFlags,
    FixNumpyDtype,
    FixPEP585CollectionNames,
    FixPybind11EnumStrDoc,
    FixRedundantBuiltinsAnnotation,
    FixRedundantMethodsFromBuiltinObject,
    FixScipyTypeArguments,
    FixTypingTypeNames,
    FixValueReprRandomAddress,
    OverridePrintSafeValues,
    RemoveSelfAnnotation,
    ReplaceReadWritePropertyWithField,
    RewritePybind11EnumValueRepr,
)
from pybind11_stubgen.parser.mixins.parse import (
    BaseParser,
    ExtractSignaturesFromPybind11Docstrings,
    ParserDispatchMixin,
)
from pybind11_stubgen.printer import Printer
from pybind11_stubgen.writer import Writer

error_handlers_top: list[type] = [
    LoggerData,
]

error_handlers_bottom: list[type] = [
    LogErrors,
    SuggestCxxSignatureFix,
]

numpy_fixes: list[type] = []


class Parser(
    *error_handlers_top,  # type: ignore[misc]
    FixMissing__future__AnnotationsImport,
    FixMissing__all__Attribute,
    FixMissingNoneHashFieldAnnotation,
    FixMissingImports,
    FilterTypingModuleAttributes,
    FixPEP585CollectionNames,
    FixTypingTypeNames,
    FixScipyTypeArguments,
    FixMissingFixedSizeImport,
    FixMissingEnumMembersAnnotation,
    OverridePrintSafeValues,
    *numpy_fixes,  # type: ignore[misc]
    FixNumpyDtype,
    FixNumpyArrayFlags,
    FixCurrentModulePrefixInTypeNames,
    FixBuiltinTypes,
    RewritePybind11EnumValueRepr,
    FilterClassMembers,
    ReplaceReadWritePropertyWithField,
    FilterInvalidIdentifiers,
    FixValueReprRandomAddress,
    FixRedundantBuiltinsAnnotation,
    FilterPybindInternals,
    FilterPybind11ViewClasses,
    FixRedundantMethodsFromBuiltinObject,
    RemoveSelfAnnotation,
    FixPybind11EnumStrDoc,
    ExtractSignaturesFromPybind11Docstrings,
    ParserDispatchMixin,
    BaseParser,
    *error_handlers_bottom,  # type: ignore[misc]
):
    pass


def main():
    output_path = Path(__file__).resolve().parent.parent
    parser = Parser()
    parser.set_pybind11_enum_locations({re.compile("RenderType"): "endstone._python.scoreboard"})
    printer = Printer(invalid_expr_as_ellipses=True)
    out_dir, sub_dir = to_output_and_subdir(
        output_dir=str(output_path),
        module_name="endstone._python",
        root_suffix=None,
    )
    run(
        parser,
        printer,
        "endstone._python",
        out_dir,
        sub_dir=sub_dir,
        dry_run=False,
        writer=Writer(),
    )

    # with open(output_path / "endstone" / "_internal" / "endstone_python.pyi", 'r',
    #           encoding='utf-8') as file:
    #     content = file.read()
    #
    # content = content.replace("endstone._internal.endstone_python.", "")
    #
    # with open(output_path / "endstone" / "_internal" / "endstone_python.pyi", 'w',
    #           encoding='utf-8') as file:
    #     file.write(content)

    print(f"Stubs generated successfully to: {output_path}")


if __name__ == "__main__":
    main()
