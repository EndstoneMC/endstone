cmake_minimum_required(VERSION 3.12)

# detours
add_library(detours STATIC
        detours/src/creatwth.cpp
        detours/src/detours.cpp
        detours/src/disasm.cpp
        detours/src/disolarm.cpp
        detours/src/disolarm64.cpp
        detours/src/disolia64.cpp
        detours/src/disolx64.cpp
        detours/src/disolx86.cpp
        detours/src/image.cpp
        detours/src/modules.cpp
)
target_include_directories(detours PUBLIC detours/src)
include(GNUInstallDirs)
install(TARGETS detours)
install(FILES detours/src/detours.h detours/src/detver.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
