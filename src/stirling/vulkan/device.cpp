#include "device.h"
#include "physical_device.h"

#include <vector>

namespace stirling {

	VulkanDevice::VulkanDevice(VkDevice device, QueueFamilyIndices indices) :
		m_device         (device),
		m_graphics_queue (initQueue(indices.graphics_family_index)),
		m_present_queue  (initQueue(indices.present_family_index)) {
	}

	VulkanQueue VulkanDevice::initQueue(int queue_family_index) {
		VkQueue queue;
		vkGetDeviceQueue(m_device, queue_family_index, 0, &queue);
		return VulkanQueue(queue);
	}

	VulkanDevice::~VulkanDevice() {
		vkDestroyDevice(m_device, nullptr);
	}

}