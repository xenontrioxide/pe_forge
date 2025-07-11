#include "pe_forge/viewer/nt_headers_view.hpp"
#include "../parse_helpers.hpp"

#include <pe_forge/executable.hpp>

#include <cstdint>
#include <stdexcept>


namespace pe::viewer::nt
{
    namespace internal
    {
        template <typename T>
        std::uint16_t get_machine(const executable& executable)
        {
            const auto nt_headers = pe::parse_helpers::get_nt_headers<T>(executable);
            return nt_headers->FileHeader.Machine;
        }

        template <typename T>
        std::uint16_t get_size_of_image(const executable& executable)
        {
            const auto nt_headers = pe::parse_helpers::get_nt_headers<T>(executable);
            return nt_headers->OptionalHeader.SizeOfImage;
        }

        template <typename T>
        std::uint16_t get_size_of_headers(const executable& executable)
        {
            const auto nt_headers = pe::parse_helpers::get_nt_headers<T>(executable);
            return nt_headers->OptionalHeader.SizeOfHeaders;
        }

        template <typename T>
        std::uint16_t get_number_of_sections(const executable& executable)
        {
            const auto nt_headers = pe::parse_helpers::get_nt_headers<T>(executable);
            return nt_headers->FileHeader.NumberOfSections;
        }

        template <typename T>
        std::uint32_t get_file_alignment(const executable& executable)
        {
            const auto nt_headers = pe::parse_helpers::get_nt_headers<T>(executable);
            return nt_headers->OptionalHeader.FileAlignment;
        }

        template <typename T>
        std::uint32_t get_section_alignment(const executable& executable)
        {
            const auto nt_headers = pe::parse_helpers::get_nt_headers<T>(executable);
            return nt_headers->OptionalHeader.SectionAlignment;
        }

        template <typename T>
        std::array<data_directory, 16> get_data_directory(const executable& executable)
        {
            const auto nt_headers = pe::parse_helpers::get_nt_headers<T>(executable);

            std::array<data_directory, 16> data_array{};
            for (int i = 0; i < data_array.size(); i++)
            {
                const auto source = nt_headers->OptionalHeader.DataDirectory[i];
                data_array[i] = data_directory{ source.VirtualAddress, source.Size };
            }

            return data_array;
        }
    }

    std::uint16_t get_magic(const executable& executable)
    {
        const auto nt_headers = pe::parse_helpers::get_nt_headers<IMAGE_NT_HEADERS>(executable);
        return nt_headers->OptionalHeader.Magic; //In both x32 and x64 Magic is the first data member with the same size
    }

    architecture get_architecture(const executable& executable)
    {
        switch (get_magic(executable))
        {
            case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
                return architecture::x32;
            case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
                return architecture::x64;
            default:
                return architecture::unsupported;
        }
    }

    std::uint16_t get_machine(const executable& executable)
    {
        const auto architecture = get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::get_machine<IMAGE_NT_HEADERS64>(executable);
            case architecture::x32:
                return internal::get_machine<IMAGE_NT_HEADERS32>(executable);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    std::uint32_t get_size_of_image(const executable& executable)
    {
        const auto architecture = get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::get_size_of_image<IMAGE_NT_HEADERS64>(executable);
            case architecture::x32:
                return internal::get_size_of_image<IMAGE_NT_HEADERS32>(executable);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    std::uint32_t get_size_of_headers(const executable& executable)
    {
        const auto architecture = get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::get_size_of_headers<IMAGE_NT_HEADERS64>(executable);
            case architecture::x32:
                return internal::get_size_of_headers<IMAGE_NT_HEADERS32>(executable);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    std::uint16_t get_number_of_sections(const executable& executable)
    {
        const auto architecture = get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::get_number_of_sections<IMAGE_NT_HEADERS64>(executable);
            case architecture::x32:
                return internal::get_number_of_sections<IMAGE_NT_HEADERS32>(executable);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    std::uint32_t get_file_alignment(const executable& executable)
    {
        const auto architecture = get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::get_file_alignment<IMAGE_NT_HEADERS64>(executable);
            case architecture::x32:
                return internal::get_file_alignment<IMAGE_NT_HEADERS32>(executable);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    std::uint32_t get_section_alignment(const executable& executable)
    {
        const auto architecture = get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::get_section_alignment<IMAGE_NT_HEADERS64>(executable);
            case architecture::x32:
                return internal::get_section_alignment<IMAGE_NT_HEADERS32>(executable);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }

    std::array<data_directory, 16> get_data_directory(const executable& executable)
    {
        const auto architecture = get_architecture(executable);
        switch (architecture)
        {
            case architecture::x64:
                return internal::get_data_directory<IMAGE_NT_HEADERS64>(executable);
            case architecture::x32:
                return internal::get_data_directory<IMAGE_NT_HEADERS32>(executable);
            default:
                throw std::runtime_error("Binary Format not supported");
        }
    }
}