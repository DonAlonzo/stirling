#pragma once

#include "vulkan/vulkan.h"

#include <vector>

namespace stirling {

	struct QueueFamilyIndices {
		int graphics_family_index;
		int present_family_index;

		bool isComplete();
	};

	class VulkanPhysicalDevice {
	public:
		VulkanPhysicalDevice(VkPhysicalDevice physical_device);
		
		operator VkPhysicalDevice() const;

		QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface) const;

	private:
		VkPhysicalDevice m_physical_device;
	};

}