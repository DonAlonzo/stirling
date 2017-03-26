#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

namespace stirling {
	namespace vulkan {
		class Buffer {
		public:
			~Buffer();
			Buffer(Buffer&&);
			Buffer(const Buffer&) = delete;
			Buffer& operator=(Buffer&&) = delete;
			Buffer& operator=(const Buffer&) = delete;

			operator VkBuffer() const;

		protected:
			const Device&  m_device;
			VkBuffer       m_buffer;
			VkDeviceMemory m_memory;

			Buffer(const Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		private:
			VkBuffer       initBuffer(VkDeviceSize size, VkBufferUsageFlags usage) const;
			VkDeviceMemory allocateMemory(VkMemoryPropertyFlags properties) const;
			uint32_t       findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		};
	}
}