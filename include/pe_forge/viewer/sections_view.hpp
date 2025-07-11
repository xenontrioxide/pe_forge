#pragma once
#include "pe_forge/executable.hpp"
#include <cstdint>
#include <expected>
#include <array>
#include <cstddef>

namespace pe::viewer
{
    std::vector<section> get_sections(const executable& executable);
    std::expected<section, std::string> get_section(const executable& executable, const std::array<std::byte, 8>& section_name);
    bool can_add_header(const executable& executable);
    std::expected<std::uint32_t, std::string> rva_to_raw_offset(const executable& executable, const std::uint32_t rva);
}