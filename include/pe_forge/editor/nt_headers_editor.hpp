#pragma once
#include "pe_forge/executable.hpp"
#include <cstdint>

namespace pe::editor
{
    void set_number_of_sections(executable& executable, const std::uint16_t new_count);
    void set_image_size(executable& executable, const std::uint32_t new_size);
    void set_headers_size(executable& executable, const std::uint32_t new_size);
}