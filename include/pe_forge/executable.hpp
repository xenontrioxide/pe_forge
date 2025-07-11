#pragma once
#include <vector>
#include <cstddef>
#include <array>
#include <string>
#include <cassert>
#include <cstdint>


namespace pe
{
    class executable
    {
    public:
        executable(const std::vector<std::byte>& bytes)
            : m_bytes(bytes)
        {
        }
        std::vector<std::byte> m_bytes;
    };

    //TODO: Don't love the naming
    struct section_data
    {
        std::uint32_t m_pointer;
        std::uint32_t m_size;
    };

    class section
    {
    public:
        section(const std::array<std::byte, 8>& name,
                const std::vector<std::byte>& data,
                const std::uint32_t characteristics,
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

        std::uint32_t get_characteristics() const
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
        std::uint32_t m_characteristics;
        section_data m_virtual;
        section_data m_raw;
    };

    class imported_function
    {
    public:
        imported_function(const std::string& name, const std::uintptr_t virtual_address) :
            m_name(name), m_virtual_address(virtual_address), m_ordinal(0)
        {
        }

        imported_function(const std::uint16_t ordinal, const std::uintptr_t virtual_address) :
            m_ordinal(ordinal), m_virtual_address(virtual_address), m_name()
        {
        }
    private:
        std::string m_name;
        std::uint16_t m_ordinal;
        std::uintptr_t m_virtual_address;
    };

    class import
    {
    public:
        import() :
            m_name(), m_functions()
        {

        }

        import(const std::string& name, const std::vector<imported_function>& functions) :
            m_name(name), m_functions(functions)
        {

        }

        std::string get_name() const
        {
            return m_name;
        }

        std::vector<imported_function> get_functions() const
        {
            return m_functions;
        }
    private:
        std::string m_name;
        std::vector<imported_function> m_functions;
    };

    struct data_directory
    {
        std::uint32_t m_virtual_address;
        std::uint32_t m_size;
    };

    enum class architecture
    {
        unsupported,
        x32,
        x64
    };
}
