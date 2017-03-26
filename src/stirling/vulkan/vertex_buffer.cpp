#include "vertex_buffer.h"
#include "device.h"
#include "../stirling.h"

namespace stirling {
	namespace vulkan {

		VertexBuffer::VertexBuffer(const Device& device, const std::vector<Vertex>& vertices) :
			m_device(device),
			m_buffer(initBuffer(vertices)),
			m_memory(allocateMemory()) {

			vkBindBufferMemory(device, m_buffer, m_memory, 0);
			void* data;
			vkMapMemory(device, m_memory, 0, sizeof(vertices[0]) * vertices.size(), 0, &data);
			memcpy(data, vertices.data(), sizeof(vertices[0]) * vertices.size());
			vkUnmapMemory(device, m_memory);
		}

		VertexBuffer::VertexBuffer(VertexBuffer&& rhs) :
			m_device(std::move(rhs.m_device)),
			m_buffer(std::move(rhs.m_buffer)),
			m_memory(std::move(rhs.m_memory)) {
			rhs.m_buffer = VK_NULL_HANDLE;
			rhs.m_memory = VK_NULL_HANDLE;
		}

		VkBuffer VertexBuffer::initBuffer(const std::vector<Vertex>& vertices) const {
			VkBufferCreateInfo create_info = {};
			create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			create_info.size        = sizeof(vertices[0]) * vertices.size();
			create_info.usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VkBuffer buffer;
			if (vkCreateBuffer(m_device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create buffer.");
			}
			return buffer;
		}

		VkDeviceMemory VertexBuffer::allocateMemory() const {
			VkMemoryRequirements memory_requirements;
			vkGetBufferMemoryRequirements(m_device, m_buffer, &memory_requirements);

			VkMemoryAllocateInfo allocate_info = {};
			allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocate_info.allocationSize  = memory_requirements.size;
			allocate_info.memoryTypeIndex = findMemoryType(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			VkDeviceMemory memory;
			if (vkAllocateMemory(m_device, &allocate_info, nullptr, &memory) != VK_SUCCESS) {
				throw std::runtime_error("Failed to allocate buffer memory.");
			}
			return memory;
		}

		uint32_t VertexBuffer::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
			VkPhysicalDeviceMemoryProperties memory_properties;
			vkGetPhysicalDeviceMemoryProperties(m_device.getPhysicalDevice(), &memory_properties);

			for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
				if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}
			throw std::runtime_error("Failed to find suitable memory type.");
		}

		VertexBuffer::~VertexBuffer() {
			if (m_buffer != VK_NULL_HANDLE) {
				vkFreeMemory(m_device, m_memory, nullptr);
				vkDestroyBuffer(m_device, m_buffer, nullptr);
			}
		}

		VertexBuffer::operator VkBuffer() const {
			return m_buffer;
		}

	}
}