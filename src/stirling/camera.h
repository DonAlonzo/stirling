#pragma once

#include "glm/glm.hpp"

#include "vulkan/transform.h"

namespace stirling {
    class Camera : public vulkan::Transform {
    public:
        Camera();
    };
}