#include "pe_forge/viewer/nt_headers_view.hpp"

#include "../parse_helpers.hpp"

#include <Windows.h>
#include <cstddef>
#include <format>
#include <array>
#include <cstdint>
#include <cstring>
#include <expected>
#include <string>
#include <vadefs.h>
#include <vector>
#include <pe_forge/executable.hpp>

namespace pe::viewer
{
    //Helper function
    const std::uintptr_t get_section_header_end(const executable& executable)
    {
        const auto num_sections = pe::viewer::nt::get_number_of_sections(executable);
        const auto pe_sections = pe::parse_helpers::get_section_headers(executable);
        return reinterpret_cast<const std::uintptr_t>(pe_sections + num_sections);
    }

    std::vector<section> get_sections(const executable& executable)
    {
        const auto num_sections = pe::viewer::nt::get_number_of_sections(executable);
        const auto pe_sections = pe::parse_helpers::get_section_headers(executable);

        std::vector<section> sections{};
        for (int i = 0; i < num_sections; ++i)
        {
            const auto pe_section = pe_sections[i];
            const auto section_bytes = std::vector<std::byte>(executable.m_bytes.data() + pe_section.PointerToRawData,
                                                              executable.m_bytes.data() + pe_section.PointerToRawData + pe_section.SizeOfRawData);
            auto section_name = std::array<std::byte, 8>{};
            std::memcpy(section_name.data(), reinterpret_cast<const std::byte*>(pe_section.Name), 8);
            section section{
                section_name,
                section_bytes,
                static_cast<section_traits::characteristics>(pe_section.Characteristics),
                pe_section.VirtualAddress,
                pe_section.Misc.VirtualSize,
                pe_section.PointerToRawData,
                pe_section.SizeOfRawData
            };
            sections.push_back(section);
        }

        return sections;
    }

    std::expected<section, std::string> get_section(const executable& executable, const std::array<std::byte, 8>& section_name)
    {
        const auto sections = get_sections(executable);
        for (const auto& section : sections)
        {
            if (section_name == section.get_name())
                return section;
        }

        return std::unexpected(std::format("Section with name couldn't be found in executable."));
    }

    bool can_add_header(const executable& executable)
    {
        const auto section_header_end_address = get_section_header_end(executable);
        const auto executable_base = reinterpret_cast<std::uintptr_t>(executable.m_bytes.data());
        const auto section_header_end_offset = section_header_end_address - executable_base;

        const auto size_of_headers = pe::viewer::nt::get_size_of_headers(executable);
        const auto available_space = size_of_headers - section_header_end_offset;

        constexpr auto required_space = sizeof(IMAGE_SECTION_HEADER);

        if (available_space < required_space)
        {
            return false;
        }

        return true;
    }

    std::expected<std::uint32_t, std::string> rva_to_raw_offset(const executable& executable, const std::uint32_t rva)
    {
        const auto sections = get_sections(executable);
        for (const auto& section : sections)
        {
            const auto start = section.get_virtual().m_pointer;
            const auto end = start + section.get_virtual().m_size;
            if (rva >= start && rva <= end)
            {
                const auto offset = rva - start;
                const auto raw_offset = section.get_raw().m_pointer + offset;
                return raw_offset;
            }
        }

        return std::unexpected("RVA didn't live within any sections.");
    }
}