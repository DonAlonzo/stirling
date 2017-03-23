#include "physical_device.h"

#include <vector>

namespace stirling {

	VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physical_device) :
		m_physical_device (physical_device) {
	}


	QueueFamilyIndices VulkanPhysicalDevice::findQueueFamilies() const {
		QueueFamilyIndices indices;

		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);
		std::vector<VkQueueFamilyProperties> queue_families{queue_family_count};
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_families.data());

		for (int i = 0; i < queue_families.size(); ++i) {
			if (queue_families[i].queueCount == 0) continue;
			
			if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphics_family = i;
			}

			if (indices.isComplete()) break;
		}

		return indices;
	}

	bool QueueFamilyIndices::isComplete() const {
		return graphics_family >= 0;
	}

}