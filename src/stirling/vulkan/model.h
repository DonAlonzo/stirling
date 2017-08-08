#pragma once

#include "vertex.h"

#include <string>
#include <vector>

namespace stirling {
    namespace vulkan {

        struct Model {
            std::vector<vulkan::Vertex> vertices;
            std::vector<uint32_t> indices;

            static Model loadFromFile(const std::string& model_path);
        };

    }
}