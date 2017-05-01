#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace stirling {
    namespace util {
        namespace io {

            std::vector<char> readFile(const std::string& file_name);

        }
    }
}