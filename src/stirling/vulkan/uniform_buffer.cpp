#include "uniform_buffer.h"
#include "uniform_buffer_object.h"
#include "device.h"

namespace stirling {
	namespace vulkan {

		UniformBuffer::UniformBuffer(const Device& device) :
			Buffer(device, sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
		}

		void UniformBuffer::update(const UniformBufferObject& uniform_buffer_object) const {
			void* data;
			vkMapMemory(*m_device, m_memory, 0, sizeof(UniformBufferObject), 0, &data);
			memcpy(data, &uniform_buffer_object, sizeof(UniformBufferObject));
			vkUnmapMemory(*m_device, m_memory);
		}

	}
}