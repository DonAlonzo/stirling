#pragma once

#include <string>

namespace stirling {

    struct Material {
    };

    struct Blueprint {
        std::string model_file;
        std::string texture_file;
        std::string vertex_shader;
        std::string fragment_shader;

        void* component_blueprints;
    };

}