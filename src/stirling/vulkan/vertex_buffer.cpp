#include "vertex.h"
#include "vertex_buffer.h"
#include "device.h"

namespace stirling {
    namespace vulkan {

        VertexBuffer::VertexBuffer(const Device& device, const std::vector<Vertex>& vertices) :
            Buffer(device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(Vertex) * vertices.size()),
            m_size (vertices.size()) {

            map();
            memcpy(vertices.data());
            unmap();
        }

        int VertexBuffer::size() const {
            return m_size;
        }

    }
}