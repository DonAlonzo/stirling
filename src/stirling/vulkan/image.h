#pragma once

#include "vulkan/vulkan.h"
namespace stirling {
	namespace vulkan {
		class Device;
	}
}

#include <string>

namespace stirling {
	namespace vulkan {
		class Image {
		public:
			Image(const Device& device, const VkImageCreateInfo& create_info, const VkMemoryPropertyFlags& properties);
			~Image();
			Image(Image&&);
			Image(const Image&) = delete;
			Image& operator=(Image&&) = delete;
			Image& operator=(const Image&) = delete;

			static Image loadImage(const Device& device, const std::string& file_name);

		private:
			const Device&  m_device;
			VkImage        m_image;
			VkDeviceMemory m_memory;

			VkImage        initImage(const VkImageCreateInfo& create_info);
			VkDeviceMemory allocateMemory(const VkMemoryPropertyFlags& properties);
			uint32_t       findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		};
	}
}