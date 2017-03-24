#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "device.h"
namespace stirling {
	class VulkanSurface;
}

// std
#include <vector>

namespace stirling {

	struct QueueFamilyIndices {
		int graphics_family_index;
		int present_family_index;

		bool isComplete() const;
	};

	class VulkanPhysicalDevice {
	public:
		VulkanPhysicalDevice(VkPhysicalDevice physical_device);
		
		operator VkPhysicalDevice() const;

		QueueFamilyIndices findQueueFamilies(const VulkanSurface& surface) const;

		VulkanDevice createDevice(const VulkanSurface& surface, const std::vector<const char*> extensions) const;

		std::vector<VkExtensionProperties> getExtensions() const;

	private:
		VkPhysicalDevice m_physical_device;
	};

}