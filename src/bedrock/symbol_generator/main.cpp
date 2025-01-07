#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <toml++/toml.h>

namespace {
void generate_include_file(const std::string &output_file, const toml::table &table)
{
    namespace fs = std::filesystem;

    fs::path output_path = output_file;
    fs::path parent_dir = output_path.parent_path();

    // Create parent directories if they don't exist
    if (!parent_dir.empty() && !fs::exists(parent_dir)) {
        try {
            fs::create_directories(parent_dir);
        } catch (const fs::filesystem_error &e) {
            std::cerr << "Failed to create directories for output file: " << e.what() << '\n';
            return;
        }
    }

    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "Failed to open output file: " << output_file << '\n';
        return;
    }

    output << "#pragma once\n\n";
    output << "#include <array>\n#include <string_view>\n\n";
    output << "static constexpr std::array<std::pair<std::string_view, std::size_t>, " << table.size()
           << "> symbols = {{\n";

    for (const auto &[key, value] : table) {
        if (!value.is_integer()) {
            std::cerr << "Skipping non-integer value for key: " << key << '\n';
            continue;
        }
        output << "    { \"" << key << "\", " << *value.value<int64_t>() << " },\n";
    }

    output << "}};\n";
    output.close();
    std::cout << "Include file generated: " << output_file << '\n';
}
}  // namespace

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.toml> <symbols.inc>" << '\n';
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    try {
        auto table = toml::parse_file(input_file);
#ifdef _WIN32
        generate_include_file(output_file, *table["windows"].as_table());
#elif __linux__
        generate_include_file(output_file, *table["linux"].as_table());
#else
#error Unsupported platform
#endif
    }
    catch (const toml::parse_error &err) {
        std::cerr << "Failed to parse TOML file: " << err.description() << '\n';
        return 1;
    }

    return 0;
}
