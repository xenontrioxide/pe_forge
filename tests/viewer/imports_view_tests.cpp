#include <gtest/gtest.h>
#include "pe_forge/viewer/imports_view.hpp"

#include "test_helpers.hpp"

TEST(imports, get_imports)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto imports = pe::viewer::get_imports(exe);
    EXPECT_EQ(imports.size(), 0x9);
}

TEST(imports, find_import)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto imports = pe::viewer::get_imports(exe);
    bool found = false;
    pe::import kernel32{};
    for (const auto& import : imports)
    {
        if (import.get_name() == "KERNEL32.dll")
        {
            found = true;
            kernel32 = import;
        }
    }

    EXPECT_TRUE(found);
    EXPECT_EQ(kernel32.get_functions().size(), 15);
}

TEST(imports, check_imports_functions)
{
    const auto executable_bytes = test_helpers::get_test_executable_bytes();
    const auto exe = pe::executable(executable_bytes);
    const auto imports = pe::viewer::get_imports(exe);
    bool found = false;
    pe::import kernel32{};
    for (const auto& import : imports)
    {
        if (import.get_name() == "KERNEL32.dll")
        {
            found = true;
            kernel32 = import;
        }
    }
    const auto& functions = kernel32.get_functions();

    EXPECT_TRUE(found);
    EXPECT_EQ(kernel32.get_functions().size(), 15);
    const auto& kernel32_first_function = functions[0];
    EXPECT_EQ(kernel32_first_function.get_name(), "RtlCaptureContext");
    EXPECT_THROW(kernel32_first_function.get_ordinal(), std::runtime_error);
    EXPECT_EQ(kernel32_first_function.get_virtual_address(), 0x3000);
}