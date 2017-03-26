#include "buffer.h"
#include "device.h"
#include "physical_device.h"

namespace stirling {
	namespace vulkan {

		Buffer::Buffer(const Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) :
			m_device (device),
			m_buffer (initBuffer(size, usage)),
			m_memory (allocateMemory(properties)) {

			vkBindBufferMemory(device, m_buffer, m_memory, 0);
		}

		Buffer::Buffer(Buffer&& rhs) :
			m_device (std::move(rhs.m_device)),
			m_buffer (std::move(rhs.m_buffer)),
			m_memory (std::move(rhs.m_memory)) {
			rhs.m_buffer = VK_NULL_HANDLE;
			rhs.m_memory = VK_NULL_HANDLE;
		}

		VkBuffer Buffer::initBuffer(VkDeviceSize size, VkBufferUsageFlags usage) const {
			VkBufferCreateInfo create_info = {};
			create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			create_info.size        = size;
			create_info.usage       = usage;
			create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VkBuffer buffer;
			if (vkCreateBuffer(m_device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create buffer.");
			}
			return buffer;
		}

		VkDeviceMemory Buffer::allocateMemory(VkMemoryPropertyFlags properties) const {
			VkMemoryRequirements memory_requirements;
			vkGetBufferMemoryRequirements(m_device, m_buffer, &memory_requirements);

			VkMemoryAllocateInfo allocate_info = {};
			allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocate_info.allocationSize  = memory_requirements.size;
			allocate_info.memoryTypeIndex = findMemoryType(memory_requirements.memoryTypeBits, properties);

			VkDeviceMemory memory;
			if (vkAllocateMemory(m_device, &allocate_info, nullptr, &memory) != VK_SUCCESS) {
				throw std::runtime_error("Failed to allocate buffer memory.");
			}
			return memory;
		}

		uint32_t Buffer::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
			VkPhysicalDeviceMemoryProperties memory_properties;
			vkGetPhysicalDeviceMemoryProperties(m_device.getPhysicalDevice(), &memory_properties);

			for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i) {
				if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}
			throw std::runtime_error("Failed to find suitable memory type.");
		}

		Buffer::~Buffer() {
			if (m_buffer != VK_NULL_HANDLE) {
				vkFreeMemory(m_device, m_memory, nullptr);
				vkDestroyBuffer(m_device, m_buffer, nullptr);
			}
		}

		Buffer::operator VkBuffer() const {
			return m_buffer;
		}

	}
}