#include "physical_device.h"
#include "device.h"

#include <vector>

namespace stirling {
	
	VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physical_device) :
		m_physical_device (physical_device) {
	}

	VulkanPhysicalDevice::operator VkPhysicalDevice() const {
		return m_physical_device;
	}

	QueueFamilyIndices VulkanPhysicalDevice::findQueueFamilies(VkSurfaceKHR surface) const {
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);
		std::vector<VkQueueFamilyProperties> queue_families{queue_family_count};
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_families.data());

		QueueFamilyIndices indices;
		for (int i = 0; i < queue_families.size(); ++i) {
			if (queue_families[i].queueCount == 0) continue;
			
			if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphics_family_index = i;
			}

			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, surface, &present_support);

			if (queue_families[i].queueCount > 0 && present_support) {
				indices.present_family_index = i;
			}

			if (indices.isComplete()) break;
		}
		return indices;
	}

	bool QueueFamilyIndices::isComplete() {
		return graphics_family_index >= 0
			&& present_family_index >= 0;
	}

}