#include <gtest/gtest.h>
#include "pe_forge/viewer/sections_view.hpp"

#include "test_helpers.hpp"

#include <array>
#include <cstddef>

//Sections within test.bin
constexpr auto text_section_name = std::array<std::byte, 8>{
        std::byte('.'),
        std::byte('t'),
        std::byte('e'),
        std::byte('x'),
        std::byte('t'),
        std::byte('\0'),
        std::byte('\0'),
        std::byte('\0')
};

constexpr auto rdata_section_name = std::array<std::byte, 8>{
        std::byte('.'),
        std::byte('r'),
        std::byte('d'),
        std::byte('a'),
        std::byte('t'),
        std::byte('a'),
        std::byte('\0'),
        std::byte('\0')
};

constexpr auto data_section_name = std::array<std::byte, 8>{
        std::byte('.'),
        std::byte('d'),
        std::byte('a'),
        std::byte('t'),
        std::byte('a'),
        std::byte('\0'),
        std::byte('\0'),
        std::byte('\0')
};

constexpr auto pdata_section_name = std::array<std::byte, 8>{
        std::byte('.'),
        std::byte('p'),
        std::byte('d'),
        std::byte('a'),
        std::byte('t'),
        std::byte('a'),
        std::byte('\0'),
        std::byte('\0')
};

constexpr auto rsrc_section_name = std::array<std::byte, 8>{
        std::byte('.'),
        std::byte('r'),
        std::byte('s'),
        std::byte('r'),
        std::byte('c'),
        std::byte('\0'),
        std::byte('\0'),
        std::byte('\0')
};

constexpr auto reloc_section_name = std::array<std::byte, 8>{
        std::byte('.'),
        std::byte('r'),
        std::byte('e'),
        std::byte('l'),
        std::byte('o'),
        std::byte('c'),
        std::byte('\0'),
        std::byte('\0')
};

TEST(sections, get_sections)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto sections = pe::viewer::get_sections(exe);
    EXPECT_EQ(sections.size(), 0x6);
    EXPECT_EQ(sections[0].get_name(), text_section_name);
    EXPECT_EQ(sections[1].get_name(), rdata_section_name);
    EXPECT_EQ(sections[2].get_name(), data_section_name);
    EXPECT_EQ(sections[3].get_name(), pdata_section_name);
    EXPECT_EQ(sections[4].get_name(), rsrc_section_name);
    EXPECT_EQ(sections[5].get_name(), reloc_section_name);
}

TEST(sections, get_section_valid)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto section_result = pe::viewer::get_section(exe, text_section_name);
    EXPECT_TRUE(section_result.has_value());
    EXPECT_EQ(section_result->get_name(), text_section_name);
    EXPECT_EQ(section_result->get_raw().m_pointer, 0x400);
    EXPECT_EQ(section_result->get_raw().m_size, 0x1200);
    EXPECT_EQ(section_result->get_virtual().m_pointer, 0x1000);
    EXPECT_EQ(section_result->get_virtual().m_size, 0x10C9);
    EXPECT_EQ(section_result->get_characteristics(), 0x6000'0020);
}

TEST(sections, get_section_invalid)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    constexpr std::array<std::byte, 8> invalid_section_name = {
        std::byte('.'),
        std::byte('n'),
        std::byte('o'),
        std::byte('n'),
        std::byte('e'),
        std::byte('\0'),
        std::byte('\0'),
        std::byte('\0')
    };

    const auto section_result = pe::viewer::get_section(exe, invalid_section_name);
    EXPECT_FALSE(section_result.has_value());
}
