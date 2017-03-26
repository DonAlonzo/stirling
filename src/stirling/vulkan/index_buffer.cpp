#include "index_buffer.h"
#include "device.h"
#include "../stirling.h"

namespace stirling {
	namespace vulkan {

		IndexBuffer::IndexBuffer(const Device& device, const std::vector<uint16_t>& indices) :
			Buffer(device, sizeof(uint16_t) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
			m_size (indices.size()) {

			void* data;
			vkMapMemory(device, m_memory, 0, sizeof(uint16_t) * size(), 0, &data);
			memcpy(data, indices.data(), sizeof(uint16_t) * size());
			vkUnmapMemory(device, m_memory);
		}

		int IndexBuffer::size() const {
			return m_size;
		}

	}
}