cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

target_include_directories(pe_forge PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_executable(pe_forge_tests
    "main.cpp"
    "viewer/dos_header_view_tests.cpp" 
    "viewer/imports_view_tests.cpp" 
    "viewer/nt_headers_view_tests.cpp"
 "viewer/sections_views_tests.cpp" "editor/nt_headers_editor_tests.cpp" "editor/sections_editor_tests.cpp")

target_link_libraries(pe_forge_tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        pe_forge
)

# Copy test binary to output for running
add_custom_command(
    TARGET pe_forge_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/test.bin
    $<TARGET_FILE_DIR:pe_forge_tests>
)

gtest_discover_tests(pe_forge_tests)