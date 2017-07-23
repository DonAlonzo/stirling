#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "buffer.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        struct IndexBuffer : public Buffer {
            IndexBuffer(const Device* device, const std::vector<uint32_t>& indices);
        };
    }
}