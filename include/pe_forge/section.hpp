#pragma once
#include <utility>
#include <array>
#include <cassert>

namespace pe
{
    //TODO: Don't love the naming
    struct section_data
    {
        std::uint32_t m_pointer;
        std::uint32_t m_size;
    };

    struct section_traits
    {
        enum class characteristics : std::uint32_t
        {
            none = 0x00000000,

            // Reserved types
            type_dsect = 0x00000001,
            type_noload = 0x00000002,
            type_group = 0x00000004,
            type_no_pad = 0x00000008,
            type_copy = 0x00000010,

            // Section contents
            cnt_code = 0x00000020,
            cnt_initialized_data = 0x00000040,
            cnt_uninitialized_data = 0x00000080,

            // Linker flags
            lnk_other = 0x00000100,
            lnk_info = 0x00000200,
            lnk_remove = 0x00000800,
            lnk_comdat = 0x00001000,

            // Misc
            no_defer_spec_exc = 0x00004000,
            gprel = 0x00008000,
            mem_fardata = 0x00008000,
            mem_purgeable = 0x00020000,
            mem_16bit = 0x00020000,
            mem_locked = 0x00040000,
            mem_preload = 0x00080000,

            // Alignment
            align_1bytes = 0x00100000,
            align_2bytes = 0x00200000,
            align_4bytes = 0x00300000,
            align_8bytes = 0x00400000,
            align_16bytes = 0x00500000,
            align_32bytes = 0x00600000,
            align_64bytes = 0x00700000,
            align_128bytes = 0x00800000,
            align_256bytes = 0x00900000,
            align_512bytes = 0x00A00000,
            align_1024bytes = 0x00B00000,
            align_2048bytes = 0x00C00000,
            align_4096bytes = 0x00D00000,
            align_8192bytes = 0x00E00000,

            align_mask = 0x00F00000,

            // Relocation and memory flags
            lnk_nreloc_ovfl = 0x01000000,
            mem_discardable = 0x02000000,
            mem_not_cached = 0x04000000,
            mem_not_paged = 0x08000000,
            mem_shared = 0x10000000,
            mem_execute = 0x20000000,
            mem_read = 0x40000000,
            mem_write = 0x80000000
        };
    };

    inline section_traits::characteristics operator|(section_traits::characteristics a, section_traits::characteristics b)
    {
        return static_cast<section_traits::characteristics>(std::to_underlying(a) | std::to_underlying(b));
    }

    inline section_traits::characteristics operator&(section_traits::characteristics a, section_traits::characteristics b)
    {
        return static_cast<section_traits::characteristics>(std::to_underlying(a) & std::to_underlying(b));
    }

    inline section_traits::characteristics operator|=(section_traits::characteristics a, section_traits::characteristics b)
    {
        a = a | b;
        return a;
    }

    class section
    {
    public:
        section(const std::array<std::byte, 8>& name,
                const std::vector<std::byte>& data,
                const section_traits::characteristics characteristics,
                const std::uint32_t virtual_pointer,
                const std::uint32_t virtual_size,
                const std::uint32_t raw_pointer,
                const std::uint32_t raw_size) :
            m_name(name),
            m_data(data),
            m_characteristics(characteristics),
            m_virtual{ virtual_pointer, virtual_size },
            m_raw{ raw_pointer, raw_size }
        {
        }

        const std::array<std::byte, 8> get_name() const
        {
            return m_name;
        }

        const std::vector<std::byte>& get_data() const
        {
            return m_data;
        }

        section_traits::characteristics get_characteristics() const
        {
            return m_characteristics;
        }

        section_data get_virtual() const
        {
            return m_virtual;
        }

        section_data get_raw() const
        {
            return m_raw;
        }

    private:
        std::array<std::byte, 8> m_name;
        std::vector<std::byte> m_data;
        section_traits::characteristics m_characteristics;
        section_data m_virtual;
        section_data m_raw;
    };

    consteval std::array<std::byte, 8> make_section_name(const char* str)
    {
        std::array<std::byte, 8> result{};
        std::size_t i = 0;
        while (str[i] != '\0')
        {
            if (i >= 8)
                throw "Section name must be 8 characters or fewer.";

            result[i] = static_cast<std::byte>(str[i]);
            ++i;
        }

        return result;
    }

}