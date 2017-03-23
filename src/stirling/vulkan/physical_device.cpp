#include "physical_device.h"
#include "device.h"

#include <vector>

namespace stirling {

	VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physical_device) :
		m_physical_device (physical_device) {
		initQueueFamilyIndices();
	}

	bool VulkanPhysicalDevice::hasAllQueueFamilies() const {
		return m_graphics_family_index != -1;
	}

	int VulkanPhysicalDevice::getGraphicsFamilyIndex() const {
		return m_graphics_family_index;
	}

	VulkanPhysicalDevice::operator VkPhysicalDevice() const {
		return m_physical_device;
	}

	void VulkanPhysicalDevice::initQueueFamilyIndices() {
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);
		std::vector<VkQueueFamilyProperties> queue_families{queue_family_count};
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_families.data());

		for (int i = 0; i < queue_families.size(); ++i) {
			if (queue_families[i].queueCount == 0) continue;
			
			if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				m_graphics_family_index = i;
			}

			if (hasAllQueueFamilies()) break;
		}
	}

}