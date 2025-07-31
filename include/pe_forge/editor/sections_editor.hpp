#pragma once
#include "pe_forge/executable.hpp"
#include <cstdint>

namespace pe::editor
{
    void move_first_section(executable& executable, const std::uint32_t offset);
    bool add_section(executable& executable,
                     const std::array<std::byte, 8>& name,
                     const std::vector<std::byte>& data,
                     const pe::section_traits::characteristics characteristics,
                     const bool auto_move = true);
}