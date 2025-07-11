#include "pe_forge/editor/nt_headers_editor.hpp"
#include <pe_forge/executable.hpp>
#include <pe_forge/viewer/nt_headers_view.hpp>
#include "../parse_helpers.hpp"
#include <cstdint>
#include <stdexcept>
namespace pe::editor
{
    namespace internal
    {
        template <typename T>
        void set_number_of_sections(executable& executable, const std::uint16_t new_count)
        {
            pe::parse_helpers::get_nt_headers<T>(executable)->FileHeader.NumberOfSections = new_count;
        }

        template <typename T>
        void set_image_size(executable& executable, const std::uint32_t new_size)
        {
            pe::parse_helpers::get_nt_headers<T>(executable)->OptionalHeader.SizeOfImage = new_size;
        }

        template <typename T>
        void set_headers_size(executable& executable, const std::uint32_t new_size)
        {
            pe::parse_helpers::get_nt_headers<T>(executable)->OptionalHeader.SizeOfHeaders = new_size;
        }

    }
    void set_number_of_sections(executable& executable, const std::uint16_t new_count)
    {
        const auto architecture = pe::viewer::nt::get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::set_number_of_sections<IMAGE_NT_HEADERS64>(executable, new_count);
            case architecture::x32:
                return internal::set_number_of_sections<IMAGE_NT_HEADERS32>(executable, new_count);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    void set_image_size(executable& executable, const std::uint32_t new_size)
    {
        const auto architecture = pe::viewer::nt::get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::set_image_size<IMAGE_NT_HEADERS64>(executable, new_size);
            case architecture::x32:
                return internal::set_image_size<IMAGE_NT_HEADERS32>(executable, new_size);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    void set_headers_size(executable& executable, const std::uint32_t new_size)
    {
        const auto architecture = pe::viewer::nt::get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::set_headers_size<IMAGE_NT_HEADERS64>(executable, new_size);
            case architecture::x32:
                return internal::set_headers_size<IMAGE_NT_HEADERS32>(executable, new_size);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }
}