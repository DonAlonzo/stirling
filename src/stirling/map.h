#pragma once

#include <string>

namespace stirling {

    class Map {
    public:
        static Map load(const std::string& path);

    private:
        Map();
    };

}