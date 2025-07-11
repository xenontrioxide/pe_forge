#include <gtest/gtest.h>
#include "pe_forge/viewer/dos_header_view.hpp"

#include "test_helpers.hpp"

TEST(dos_header, get_magic)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto magic = pe::viewer::dos::get_magic(exe);
    EXPECT_EQ(magic, 0x5A4D);
}

TEST(dos_header, get_fanew)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto fanew = pe::viewer::dos::get_fanew(exe);
    EXPECT_EQ(fanew, 0xF0);
}
