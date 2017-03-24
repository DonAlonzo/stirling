#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "device.h"

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

		QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface) const;

		VulkanDevice createDevice(VkSurfaceKHR surface, const std::vector<const char*> extensions) const;

		std::vector<VkExtensionProperties> getExtensions() const;

	private:
		VkPhysicalDevice m_physical_device;
	};

}