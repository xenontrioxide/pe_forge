#pragma once
#include "pe_forge/executable.hpp"
#include <cstdint>
#include <vector>


namespace pe::viewer
{
    std::vector<import> get_imports(const executable& executable);
}