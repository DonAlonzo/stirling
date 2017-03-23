#pragma once

#include "vulkan/vulkan.h"

namespace stirling {

	struct QueueFamilyIndices {
		int graphics_family = -1;

		bool isComplete() const;
	};

	class VulkanPhysicalDevice {
	public:
		VulkanPhysicalDevice(VkPhysicalDevice physical_device);

		QueueFamilyIndices findQueueFamilies() const;

	private:
		VkPhysicalDevice m_physical_device;
	};

}