#include "index_buffer.h"
#include "device.h"
#include "../stirling.h"

namespace stirling {
    namespace vulkan {

        IndexBuffer::IndexBuffer(const Device* device, const std::vector<uint32_t>& indices) :
            Buffer(device, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(uint32_t) * indices.size()),
            m_size (indices.size()) {

            map();
            memcpy(indices.data());
            unmap();
        }

        size_t IndexBuffer::size() const {
            return m_size;
        }

    }
}