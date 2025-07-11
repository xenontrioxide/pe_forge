#pragma once
#include "pe_forge/executable.hpp"
#include "pe_forge/viewer/dos_header_view.hpp"
#include "pe_forge/viewer/nt_headers_view.hpp"
#include <Windows.h>
#include <stdexcept>
namespace pe
{
    namespace parse_helpers
    {
        inline const IMAGE_DOS_HEADER* get_dos_header(const executable& executable)
        {
            return reinterpret_cast<const IMAGE_DOS_HEADER*>(executable.m_bytes.data());
        }

        inline IMAGE_DOS_HEADER* get_dos_header(executable& executable)
        {
            return reinterpret_cast<IMAGE_DOS_HEADER*>(executable.m_bytes.data());
        }

        template <typename T>
        inline const T* get_nt_headers(const executable& executable)
        {
            const auto dos_header = reinterpret_cast<std::uintptr_t>(get_dos_header(executable));
            const auto nt_headers = dos_header + pe::viewer::dos::get_fanew(executable);
            return reinterpret_cast<const T*>(nt_headers);
        }

        template <typename T>
        inline T* get_nt_headers(executable& executable)
        {
            const auto dos_header = reinterpret_cast<std::uintptr_t>(get_dos_header(executable));
            const auto nt_headers = dos_header + pe::viewer::dos::get_fanew(executable);
            return reinterpret_cast<T*>(nt_headers);
        }

        inline const IMAGE_SECTION_HEADER* get_section_headers(const executable& executable)
        {
            const auto architecture = pe::viewer::nt::get_architecture(executable);
            switch (architecture)
            {
                case architecture::x64:
                    return IMAGE_FIRST_SECTION(get_nt_headers<IMAGE_NT_HEADERS64>(executable));
                case architecture::x32:
                    return IMAGE_FIRST_SECTION(get_nt_headers<IMAGE_NT_HEADERS32>(executable));
                default:
                    throw std::runtime_error("Unsupported Binary Format");
            }
        }

        inline IMAGE_SECTION_HEADER* get_section_headers(executable& executable)
        {
            const auto architecture = pe::viewer::nt::get_architecture(executable);
            switch (architecture)
            {
                case architecture::x64:
                    return IMAGE_FIRST_SECTION(get_nt_headers<IMAGE_NT_HEADERS64>(executable));
                case architecture::x32:
                    return IMAGE_FIRST_SECTION(get_nt_headers<IMAGE_NT_HEADERS32>(executable));
                default:
                    throw std::runtime_error("Unsupported Binary Format");
            }
        }
    }
}