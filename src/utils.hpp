#pragma once
#include <cstdint>

#include "pe_forge/executable.hpp"

namespace pe
{
    namespace utils
    {
        inline std::uint32_t calculate_padding(const std::uint32_t size, const std::uint32_t alignment)
        {
            const auto remainder = size % alignment;
            if (!remainder)
                return remainder;
            const auto padding = alignment - remainder;
            return padding;
        }

        inline std::uint32_t align_section(const std::uint32_t size, const std::uint32_t alignment)
        {
            const auto padding = calculate_padding(size, alignment);
            return size + padding;
        }
    }
}