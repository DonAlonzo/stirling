#include "image.h"

#include "device.h"
#include "physical_device.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

namespace stirling {
	namespace vulkan {

		Image Image::loadImage(const Device& device, const std::string& file_name) {
			int width, height, channels;
			auto pixels = stbi_load(file_name.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			VkDeviceSize image_size = width * height * 4;

			if (!pixels) {
				throw std::runtime_error("Failed to load texture image.");
			}

			VkImageCreateInfo create_info = {};
			create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			create_info.imageType     = VK_IMAGE_TYPE_2D;
			create_info.extent.width  = width;
			create_info.extent.height = height;
			create_info.extent.depth  = 1;
			create_info.mipLevels     = 1;
			create_info.arrayLayers   = 1;
			create_info.format        = VK_FORMAT_R8G8B8A8_UNORM;
			create_info.tiling        = VK_IMAGE_TILING_LINEAR;
			create_info.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
			create_info.usage         = VK_IMAGE_USAGE_SAMPLED_BIT;
			create_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
			create_info.samples       = VK_SAMPLE_COUNT_1_BIT;
			create_info.flags         = 0;
			
			return Image(device, create_info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}

		Image::Image(const Device& device, const VkImageCreateInfo& create_info, const VkMemoryPropertyFlags& properties) :
			m_device (device),
			m_image  (initImage(create_info)),
			m_memory (allocateMemory(properties)) {

			vkBindImageMemory(device, m_image, m_memory, 0);
		}

		Image::Image(Image&& rhs) :
			m_device (std::move(rhs.m_device)),
			m_image  (std::move(rhs.m_image)),
			m_memory (std::move(rhs.m_memory)) {
			rhs.m_image = VK_NULL_HANDLE;
			rhs.m_memory = VK_NULL_HANDLE;
		}

		Image::~Image() {
			if (m_image != VK_NULL_HANDLE) {
				vkFreeMemory(m_device, m_memory, nullptr);
				vkDestroyImage(m_device, m_image, nullptr);
			}
		}

		VkImage Image::initImage(const VkImageCreateInfo& create_info) {
			VkImage image;
			if (vkCreateImage(m_device, &create_info, nullptr, &image) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create image.");
			}
			return image;
		}

		VkDeviceMemory Image::allocateMemory(const VkMemoryPropertyFlags& properties) {
			VkMemoryRequirements memory_requirements;
			vkGetImageMemoryRequirements(m_device, m_image, &memory_requirements);

			VkMemoryAllocateInfo allocate_info = {};
			allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocate_info.allocationSize  = memory_requirements.size;
			allocate_info.memoryTypeIndex = findMemoryType(memory_requirements.memoryTypeBits, properties);

			VkDeviceMemory memory;
			if (vkAllocateMemory(m_device, &allocate_info, nullptr, &memory) != VK_SUCCESS) {
				throw std::runtime_error("Failed to allocate image memory.");
			}
			return memory;
		}

		uint32_t Image::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
			VkPhysicalDeviceMemoryProperties memory_properties;
			vkGetPhysicalDeviceMemoryProperties(m_device.getPhysicalDevice(), &memory_properties);

			for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i) {
				if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}
			throw std::runtime_error("Failed to find suitable memory type.");
		}

	}
}