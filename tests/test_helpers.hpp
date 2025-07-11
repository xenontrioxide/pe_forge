#include <vector>
#include <filesystem>
#include <cstddef>
#include <fstream>
namespace test_helpers
{
    template <typename T>
    inline std::vector<T> file_to_bytes(const std::filesystem::path& file_path)
    {
        if (!std::filesystem::exists(file_path))
            return {};

        auto file_stream = std::ifstream(file_path, std::ios::in | std::ios::binary);
        if (!file_stream.good() || !file_stream.is_open())
            return {};

        file_stream.seekg(0, file_stream.end);
        const auto length = file_stream.tellg();
        file_stream.seekg(0, file_stream.beg);

        std::vector<T> result{ };
        result.resize(length);

        file_stream.read((char*) result.data(), length);
        return result;
    }

    inline std::vector<std::byte> get_test_executable_bytes()
    {
        const auto test_executable_path{ std::filesystem::current_path() / "test.bin" };
        return file_to_bytes<std::byte>(test_executable_path);
    }
}


