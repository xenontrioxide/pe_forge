#include <gtest/gtest.h>
#include "pe_forge/viewer/nt_headers_view.hpp"

#include "test_helpers.hpp"

TEST(nt_headers, get_magic)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto magic = pe::viewer::nt::get_magic(exe);
    EXPECT_EQ(magic, 0x20B);
}

TEST(nt_headers, get_architecture)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto architecture = pe::viewer::nt::get_architecture(exe);
    EXPECT_EQ(architecture, pe::architecture::x64);
}

TEST(nt_headers, get_machine)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto machine = pe::viewer::nt::get_machine(exe);
    EXPECT_EQ(machine, 0x8664);
}

TEST(nt_headers, get_image_size)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto magic = pe::viewer::nt::get_size_of_image(exe);
    EXPECT_EQ(magic, 0x9000);
}

TEST(nt_headers, get_size_of_headers)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto size_of_headers = pe::viewer::nt::get_size_of_headers(exe);
    EXPECT_EQ(size_of_headers, 0x400);
}

TEST(nt_headers, get_number_of_sections)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto number_of_sections = pe::viewer::nt::get_number_of_sections(exe);
    EXPECT_EQ(number_of_sections, 0x6);
}

TEST(nt_headers, get_file_alignment)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto file_alignment = pe::viewer::nt::get_file_alignment(exe);
    EXPECT_EQ(file_alignment, 0x200);
}

TEST(nt_headers, get_section_alignment)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto section_alignment = pe::viewer::nt::get_section_alignment(exe);
    EXPECT_EQ(section_alignment, 0x1000);
}