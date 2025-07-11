#include "pe_forge/viewer/dos_header_view.hpp"
#include "../parse_helpers.hpp"
#include <cstdint>

namespace pe::viewer::dos
{
    std::uint16_t get_magic(const executable& executable)
    {
        const auto dos_header = pe::parse_helpers::get_dos_header(executable);
        return dos_header->e_magic;
    }

    std::uint32_t get_fanew(const executable& executable)
    {
        const auto dos_header = pe::parse_helpers::get_dos_header(executable);
        return dos_header->e_lfanew;
    }
}