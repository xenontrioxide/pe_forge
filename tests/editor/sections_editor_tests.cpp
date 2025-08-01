#include <gtest/gtest.h>
#include "pe_forge/viewer/sections_view.hpp"
#include "pe_forge/editor/sections_editor.hpp"

#include "test_helpers.hpp"

#include <array>
#include <cstddef>

//Sections within test.bin
constexpr auto text_section_name = pe::make_section_name(".text");
constexpr auto rdata_section_name = pe::make_section_name(".rdata");
constexpr auto data_section_name = pe::make_section_name(".data");
constexpr auto pdata_section_name = pe::make_section_name(".pdata");
constexpr auto rsrc_section_name = pe::make_section_name(".rsrc");
constexpr auto reloc_section_name = pe::make_section_name(".reloc");

TEST(sections, add_section)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    auto exe = pe::executable(executable_bytes);
    {
        const auto sections = pe::viewer::get_sections(exe);
        EXPECT_EQ(sections.size(), 0x6);
        EXPECT_EQ(sections[0].get_name(), text_section_name);
        EXPECT_EQ(sections[1].get_name(), rdata_section_name);
        EXPECT_EQ(sections[2].get_name(), data_section_name);
        EXPECT_EQ(sections[3].get_name(), pdata_section_name);
        EXPECT_EQ(sections[4].get_name(), rsrc_section_name);
        EXPECT_EQ(sections[5].get_name(), reloc_section_name);
    }


    const auto section_name = pe::make_section_name(".new");

    const std::vector<std::byte> section_data{
        std::byte('H'),
        std::byte('e'),
        std::byte('l'),
        std::byte('l'),
        std::byte('o'),
        std::byte(' '),
        std::byte('W'),
        std::byte('o'),
        std::byte('r'),
        std::byte('l'),
        std::byte('d'),
        std::byte('.'),
    };

    const pe::section_traits::characteristics characteristics =
        pe::section_traits::characteristics::mem_read | 
        pe::section_traits::characteristics::cnt_initialized_data;

    pe::editor::add_section(exe, section_name, section_data, characteristics);

    {
        const auto sections = pe::viewer::get_sections(exe);
        EXPECT_EQ(sections.size(), 0x7);
        EXPECT_EQ(sections[0].get_name(), text_section_name);
        EXPECT_EQ(sections[1].get_name(), rdata_section_name);
        EXPECT_EQ(sections[2].get_name(), data_section_name);
        EXPECT_EQ(sections[3].get_name(), pdata_section_name);
        EXPECT_EQ(sections[4].get_name(), rsrc_section_name);
        EXPECT_EQ(sections[5].get_name(), reloc_section_name);
        EXPECT_EQ(sections[6].get_name(), section_name);
    }
}

TEST(sections, move_first_section)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    auto exe = pe::executable(executable_bytes);
    {
        const auto sections = pe::viewer::get_sections(exe);
        EXPECT_EQ(sections.size(), 0x6);
        EXPECT_EQ(sections[0].get_raw().m_pointer, 0x0400);
        EXPECT_EQ(sections[1].get_raw().m_pointer, 0x1600);
        EXPECT_EQ(sections[2].get_raw().m_pointer, 0x2A00);
        EXPECT_EQ(sections[3].get_raw().m_pointer, 0x2C00);
        EXPECT_EQ(sections[4].get_raw().m_pointer, 0x2E00);
        EXPECT_EQ(sections[5].get_raw().m_pointer, 0x3000);
    }

    pe::editor::move_first_section(exe, 0x200);

    {
        const auto sections = pe::viewer::get_sections(exe);
        EXPECT_EQ(sections.size(), 0x6);
        EXPECT_EQ(sections[0].get_raw().m_pointer, 0x0600);
        EXPECT_EQ(sections[1].get_raw().m_pointer, 0x1800);
        EXPECT_EQ(sections[2].get_raw().m_pointer, 0x2C00);
        EXPECT_EQ(sections[3].get_raw().m_pointer, 0x2E00);
        EXPECT_EQ(sections[4].get_raw().m_pointer, 0x3000);
        EXPECT_EQ(sections[5].get_raw().m_pointer, 0x3200);
    }
}