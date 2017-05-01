#include "io.h"

namespace stirling {

    std::vector<char> util::io::readFile(const std::string& file_name) {
        std::ifstream file(file_name, std::ios::ate | std::ios::binary);

        if (!file.is_open()) throw std::runtime_error("Failed to open file.");

        auto file_size = (size_t)file.tellg();
        std::vector<char> buffer(file_size);
        file.seekg(0);
        file.read(buffer.data(), file_size);
        file.close();
        return buffer;
    }

}