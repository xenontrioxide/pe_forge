#pragma once
#include <vector>
#include <cstddef>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <variant>
#include <pe_forge/section.hpp>
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

    class imported_function
    {
    public:
        /// <summary>
        /// The function is imported by name.
        /// </summary>
        /// <param name="name">The name(symbol) of the function</param>
        /// <param name="virtual_address">The virtual address of the function</param>
        imported_function(const std::string& name, const std::uintptr_t virtual_address) :
            m_import_type(name), m_virtual_address(virtual_address), m_is_named(true)
        {
        }

        /// <summary>
        /// The function is imported by ordinal.
        /// </summary>
        /// <param name="ordinal">The offset(ordinal) of the function</param>
        /// <param name="virtual_address">The virtual address of the function</param>
        imported_function(const std::uint16_t ordinal, const std::uintptr_t virtual_address) :
            m_import_type(ordinal), m_virtual_address(virtual_address), m_is_named(false)
        {
        }

        std::string get_name() const
        {
            if (!m_is_named)
                throw std::runtime_error("Import doesn't have a name.");
            return std::get<std::string>(m_import_type);
        }

        std::uint16_t get_ordinal() const
        {
            if (m_is_named)
                throw std::runtime_error("Import doesn't have an ordinal.");
            return std::get<std::uint16_t>(m_import_type);
        }

        std::uintptr_t get_virtual_address() const
        {
            return m_virtual_address;
        }

        bool is_named() const
        {
            return m_is_named;
        }

        bool is_ordinal() const
        {
            return !m_is_named;
        }
    private:
        bool m_is_named;
        std::variant<std::string, std::uint16_t> m_import_type;
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
