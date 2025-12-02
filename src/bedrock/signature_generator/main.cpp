#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <toml++/toml.h>

namespace {
int generate_include_file(const std::string &output_file, const toml::table &table)
{
    namespace fs = std::filesystem;

    fs::path output_path = output_file;
    fs::path parent_dir = output_path.parent_path();

    if (!parent_dir.empty() && !fs::exists(parent_dir)) {
        try {
            fs::create_directories(parent_dir);
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Failed to create directories for output file: " << e.what() << '\n';
            return 1;
        }
    }

    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "Failed to open output file: " << output_file << '\n';
        return 1;
    }

    output << "#pragma once\n\n";
    output << "#include <array>\n#include <string_view>\n\n";
    output << "struct SignatureItem {\n";
    output << "    std::string_view name;\n";
    output << "    bool relative;\n";
    output << "    bool rip_relative;\n";
    output << "    int rip_offset;\n";
    output << "    int extra;\n";
    output << "    std::string_view pattern;\n";
    output << "};\n\n";

    auto *arr = table["signatures"].as_array();
    if (!arr) {
        std::cerr << "Missing [signatures] array" << '\n';
        return 1;
    }

    output << "static constexpr std::array<SignatureItem, " << arr->size() << "> signatures = {{\n";
    for (const auto &elem : *arr) {
        auto *sig = elem.as_table();
        if (!sig) continue;

        auto name = sig->operator[]("name").value<std::string>().value_or("");
        auto pattern = sig->operator[]("pattern").value<std::string>().value_or("");
        auto relative = sig->operator[]("relative").value<bool>().value_or(false);
        auto rip_relative = sig->operator[]("rip_relative").value<bool>().value_or(false);
        auto rip_offset = static_cast<int>(sig->operator[]("rip_offset").value<int64_t>().value_or(0));
        auto extra = static_cast<int>(sig->operator[]("extra").value<int64_t>().value_or(0));

        if (name.empty() || pattern.empty()) {
            std::cerr << "Skipping invalid signature entry (missing name/pattern)" << '\n';
            continue;
        }
        output << "    { \"" << name << "\", " << (relative ? "true" : "false") << ", "
               << (rip_relative ? "true" : "false") << ", " << rip_offset << ", " << extra << ", \"" << pattern
               << "\" },\n";
    }
    output << "}};\n";
    output.close();
    return 0;
}
}  // namespace

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.toml> <output.h>" << '\n';
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    try {
        auto table = toml::parse_file(input_file);
#ifdef _WIN32
        auto *plat = table["windows"].as_table();
#elif __linux__
        auto *plat = table["linux"].as_table();
#else
#error Unsupported platform
#endif
        if (!plat) {
            std::cerr << "Missing platform section in signatures.toml" << '\n';
            return 1;
        }
        return generate_include_file(output_file, *plat);
    } catch (const toml::parse_error &err) {
        std::cerr << "Failed to parse TOML file: " << err.description() << '\n';
        return 1;
    }
}
