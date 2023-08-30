# ------------------------
# Endstone API
# ------------------------

FetchContent_Declare(
        fmt
        URL https://github.com/fmtlib/fmt/releases/download/10.1.1/fmt-10.1.1.zip
)
FetchContent_MakeAvailable(fmt)

add_library(api INTERFACE)
add_library(endstone::api ALIAS api)
target_include_directories(api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(api INTERFACE fmt::fmt)