#include "vertex_buffer.h"
#include "device.h"
#include "../stirling.h"

namespace stirling {
	namespace vulkan {

		VertexBuffer::VertexBuffer(const Device& device, const std::vector<Vertex>& vertices) :
			Buffer(device, sizeof(Vertex) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
			m_size (vertices.size()) {
			void* data;
			vkMapMemory(device, m_memory, 0, sizeof(Vertex) * size(), 0, &data);
			memcpy(data, vertices.data(), sizeof(Vertex) * size());
			vkUnmapMemory(device, m_memory);
		}

		int VertexBuffer::size() const {
			return m_size;
		}

	}
}