cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(pe_forge VERSION 0.0.1 LANGUAGES CXX)

add_library(pe_forge
            STATIC
            "src/editor/nt_headers_editor.cpp"
            "src/editor/sections_editor.cpp"
            "src/viewer/dos_header_view.cpp"
            "src/viewer/imports_view.cpp"
            "src/viewer/nt_headers_view.cpp"
            "src/viewer/sections_view.cpp"
)

target_include_directories(pe_forge PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(DIRECTORY "include/pe_forge"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pe_forge
)

install(TARGETS pe_forge EXPORT pe_forgeTargets)

install(EXPORT pe_forgeTargets
    FILE pe_forgeTargets.cmake
    NAMESPACE pe_forge::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pe_forge
)

option(PEFORGE_BUILT_TESTS "Enable pe_forge tests." ON)

if(PEFORGE_BUILT_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()