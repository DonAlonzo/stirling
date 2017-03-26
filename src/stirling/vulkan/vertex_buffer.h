#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	struct Vertex;
	namespace vulkan {
		class Device;
	}
}

#include <vector>

namespace stirling {
	namespace vulkan {
		class VertexBuffer {
		public:
			VertexBuffer(const Device& device, const std::vector<Vertex>& vertices);
			~VertexBuffer();
			VertexBuffer(VertexBuffer&&);
			VertexBuffer(const VertexBuffer&) = delete;
			VertexBuffer& operator=(VertexBuffer&&) = delete;
			VertexBuffer& operator=(const VertexBuffer&) = delete;

			operator VkBuffer() const;

			int size() const;

		private:
			const Device&  m_device;
			int            m_size;
			VkBuffer       m_buffer;
			VkDeviceMemory m_memory;

			VkBuffer       initBuffer() const;
			VkDeviceMemory allocateMemory() const;
			uint32_t       findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		};
	}
}