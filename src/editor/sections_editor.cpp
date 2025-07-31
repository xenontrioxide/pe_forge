#include "pe_forge/editor/sections_editor.hpp"
#include "pe_forge/editor/nt_headers_editor.hpp"

#include "pe_forge/viewer/nt_headers_view.hpp"
#include "pe_forge/viewer/sections_view.hpp"

#include <pe_forge/executable.hpp>
#include "../parse_helpers.hpp"
#include "../utils.hpp"
#include <cstring>
#include <cstddef>

namespace pe::editor
{
    void move_first_section(executable& executable, const std::uint32_t offset)
    {
        const auto file_alignment = pe::viewer::nt::get_file_alignment(executable);
        const auto aligned_amount = pe::utils::align_section(offset, file_alignment);

        executable.m_bytes.resize(executable.m_bytes.size() + aligned_amount, std::byte(0));

        const auto number_of_sections = pe::viewer::nt::get_number_of_sections(executable);
        auto sections = pe::parse_helpers::get_section_headers(executable);

        for (int i = number_of_sections - 1; i >= 0; --i)
        {
            auto& section = sections[i];
            if (!section.SizeOfRawData) //Virtual section
                continue;

            std::vector<uint8_t> section_bytes_copy{};
            section_bytes_copy.reserve(section.SizeOfRawData);
            //Copy the data
            std::memcpy(section_bytes_copy.data(), executable.m_bytes.data() + section.PointerToRawData, section.SizeOfRawData);
            //Null out the old data
            std::memset(executable.m_bytes.data() + section.PointerToRawData, 0, section.SizeOfRawData);
            //Move the data to where it's supposed to be
            std::memcpy(executable.m_bytes.data() + section.PointerToRawData + aligned_amount, section_bytes_copy.data(), section.SizeOfRawData);

            section.PointerToRawData = section.PointerToRawData + aligned_amount;
        }

        const auto new_image_size = pe::viewer::nt::get_size_of_image(executable) + aligned_amount;
        pe::editor::set_image_size(executable, new_image_size);

        const auto new_headers_size = pe::viewer::nt::get_size_of_headers(executable) + aligned_amount;
        pe::editor::set_headers_size(executable, new_headers_size);
    }

    //This function needs to be refactored so that the resize comes
    //  first. Currently the lifetime gets shifted at the end.
    //  
    //  It should be rewritten to resize, then copy in all the data
    bool add_section(executable& executable, 
                     const std::array<std::byte, 8>& name, 
                     const std::vector<std::byte>& data, 
                     const pe::section_traits::characteristics characteristics,
                     const bool auto_move)
    {
        const auto file_alignment = pe::viewer::nt::get_file_alignment(executable);
        const auto section_alignment = pe::viewer::nt::get_section_alignment(executable);
        const auto number_of_sections = pe::viewer::nt::get_number_of_sections(executable);

        if (!pe::viewer::can_add_header(executable))
        {
            if (auto_move)
                move_first_section(executable, file_alignment);
            else
                return false;
        }

        auto sections = pe::parse_helpers::get_section_headers(executable);
        const auto last_section = &sections[number_of_sections - 1];

        //TODO: Break into sub functions
        //  1. Create new section
        //  2. Update old section
        //  3. Edit the current binary
        //  4. Copy new section data in

        //Most of the time in the NTHeaders there is space
        //  for this section to live. If you didn't auto move 
        //  the sections for the new one to fit you'll need to
        //  use move_first_section which is provided above
        //  to create the space for the new section header.
        auto new_section = &sections[number_of_sections];
        std::memset(new_section, 0, sizeof(IMAGE_SECTION_HEADER));

        //1. Start by assigning our data to the new section
        std::memcpy(new_section->Name, name.data(), name.max_size());
        const auto raw_data_offset = new_section->PointerToRawData = last_section->PointerToRawData + last_section->SizeOfRawData;
        new_section->SizeOfRawData = pe::utils::align_section(data.size(), file_alignment); //Sections need to be aligned correctly
        new_section->Characteristics = std::to_underlying(characteristics);
        new_section->VirtualAddress = last_section->VirtualAddress + pe::utils::align_section(last_section->Misc.VirtualSize, section_alignment);
        new_section->Misc.VirtualSize = data.size();

        //2. Often the old sizes are not aligned correctly, fix it
        last_section->Misc.VirtualSize = pe::utils::align_section(last_section->Misc.VirtualSize, section_alignment);

        //3. Copy data to the correct spots
        pe::editor::set_number_of_sections(executable, number_of_sections + 1);

        const auto new_size = pe::utils::align_section(new_section->VirtualAddress + new_section->Misc.VirtualSize, section_alignment);
        pe::editor::set_image_size(executable, new_size);

        const auto new_physical_size = new_section->PointerToRawData + new_section->SizeOfRawData;
        if (new_physical_size > executable.m_bytes.size())
        {
            //After this resize all of the pointers above are invalidated.
            //  you must use either new data or 'cache' relevant data
            executable.m_bytes.resize(new_physical_size, std::byte(0));
        }

        //4. Write the section data
        std::memcpy(executable.m_bytes.data() + raw_data_offset, data.data(), data.size());

        return true;
    }
}