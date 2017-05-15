#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "buffer.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        class IndexBuffer : public Buffer {
        public:
            IndexBuffer(const Device* device, const std::vector<uint32_t>& indices);

            size_t size() const;

        private:
            size_t m_size;
        };
    }
}