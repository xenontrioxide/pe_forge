#include "pe_forge/viewer/imports_view.hpp"
#include "pe_forge/viewer/nt_headers_view.hpp"
#include "pe_forge/viewer/sections_view.hpp"

#include "../parse_helpers.hpp"
#include "../utils.hpp"
#include <cstdint>

namespace pe::viewer
{
    template <typename T>
    std::vector<imported_function> functions_from_thunk_data(const executable& executable, const IMAGE_IMPORT_DESCRIPTOR* import_descriptor, const std::uint64_t ordinal_flag)
    {
        std::vector<imported_function> functions{};
        const auto lookup_table_rva = import_descriptor->OriginalFirstThunk;
        const auto lookup_table_raw_offset_result = pe::viewer::rva_to_raw_offset(executable, lookup_table_rva);
        if (!lookup_table_raw_offset_result)
            return {};

        const auto lookup_table_raw_offset = lookup_table_raw_offset_result.value();

        //For the imports actual virtual address
        const auto iat_rva = import_descriptor->FirstThunk;
        const auto iat_offset_result = pe::viewer::rva_to_raw_offset(executable, iat_rva);
        if (!iat_offset_result)
            return {};
        const auto iat_raw_offset = iat_offset_result.value();

        const auto thunks = reinterpret_cast<const T*>(executable.m_bytes.data() + lookup_table_raw_offset);
        const auto iat_thunks = reinterpret_cast<const T*>(executable.m_bytes.data() + iat_raw_offset);
        for (std::size_t i = 0; thunks[i].u1.AddressOfData; ++i)
        {
            const auto function_import = thunks[i];
            const auto iat_function_import = iat_thunks[i];
            const auto function_rva = iat_rva + i * sizeof(iat_function_import.u1.Function);
            if (function_import.u1.Ordinal & ordinal_flag)
            {
                functions.push_back(imported_function{ function_import.u1.Ordinal & 0xFFFF, function_rva });
            }
            else
            {
                const auto import_by_name_rva = function_import.u1.AddressOfData;
                const auto import_by_name_offset_result = pe::viewer::rva_to_raw_offset(executable, import_by_name_rva);
                if (!import_by_name_offset_result)
                    continue;

                const auto import_by_name_offset = import_by_name_offset_result.value();

                const auto import_by_name = reinterpret_cast<const IMAGE_IMPORT_BY_NAME*>(executable.m_bytes.data() + import_by_name_offset);
                const auto function_name = std::string(reinterpret_cast<const char*>(import_by_name->Name));
                functions.push_back(imported_function{ function_name, function_rva });
            }
        }

        return functions;
    }

    std::vector<imported_function> get_functions(const executable& executable, const IMAGE_IMPORT_DESCRIPTOR* import)
    {
        if (pe::viewer::nt::get_architecture(executable) == architecture::x64)
        {
            return functions_from_thunk_data<IMAGE_THUNK_DATA64>(executable, import, IMAGE_ORDINAL_FLAG64);
        }
        else
        {
            return functions_from_thunk_data<IMAGE_THUNK_DATA32>(executable, import, IMAGE_ORDINAL_FLAG32);
        }
    }

    std::vector<import> get_imports(const executable& executable)
    {
        std::vector<import> imports{};

        const auto data_directory = pe::viewer::nt::get_data_directory(executable);
        const auto import_table_rva = data_directory[IMAGE_DIRECTORY_ENTRY_IMPORT].m_virtual_address;
        const auto import_raw_offset_result = pe::viewer::rva_to_raw_offset(executable, import_table_rva);
        if (!import_raw_offset_result)
            return {};
        const auto import_raw_offset = import_raw_offset_result.value();

        //Each dll
        auto import_descriptor = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(executable.m_bytes.data() + import_raw_offset);
        while (import_descriptor->Name)
        {
            const auto name_rva = import_descriptor->Name;
            const auto name_rva_result = pe::viewer::rva_to_raw_offset(executable, name_rva);
            if (!name_rva_result)
                return {};

            const auto name_offset = name_rva_result.value();
            const auto name = std::string(reinterpret_cast<const char*>(executable.m_bytes.data() + name_offset));
            imports.push_back(import{ name, get_functions(executable, import_descriptor) });
            import_descriptor++;
        }

        return imports;
    }
}