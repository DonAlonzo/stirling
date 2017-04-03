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
			Image& operator=(Image&&);
			Image& operator=(const Image&) = delete;

			static Image loadFromFile(const Device& device, const std::string& file_name);

			operator VkImage() const;

			VkDeviceMemory getMemory() const;

		private:
			const Device*  m_device;
			VkImage        m_image;
			VkDeviceMemory m_memory;

			VkImage        initImage(const VkImageCreateInfo& create_info);
			VkDeviceMemory allocateMemory(const VkMemoryPropertyFlags& properties);
			uint32_t       findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		};
	}
}