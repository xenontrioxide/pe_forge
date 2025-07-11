#pragma once
#include "pe_forge/executable.hpp"
#include <cstdint>
#include <array>

namespace pe::viewer::nt
{
    std::uint16_t get_magic(const executable& executable);
    architecture get_architecture(const executable& executable);
    std::uint16_t get_machine(const executable& executable);
    std::uint32_t get_size_of_image(const executable& executable);
    std::uint32_t get_size_of_headers(const executable& executable);
    std::uint16_t get_number_of_sections(const executable& executable);
    std::uint32_t get_file_alignment(const executable& executable);
    std::uint32_t get_section_alignment(const executable& executable);
    std::array<data_directory, 16> get_data_directory(const executable& executable);
}