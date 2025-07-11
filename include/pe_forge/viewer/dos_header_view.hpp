#pragma once
#include "pe_forge/executable.hpp"
#include <cstdint>


namespace pe::viewer::dos
{
    std::uint16_t get_magic(const executable& executable);
    std::uint32_t get_fanew(const executable& executable);
}