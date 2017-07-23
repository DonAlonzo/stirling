#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        struct Vertex;
        class Device;
    }
}
#include "buffer.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        struct VertexBuffer : public Buffer {
            VertexBuffer(const Device* device, const std::vector<Vertex>& vertices);

        };
    }
}