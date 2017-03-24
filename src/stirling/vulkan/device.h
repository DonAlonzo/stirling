#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "queue.h"
namespace stirling {
	class QueueFamilyIndices;
}

namespace stirling {

	class VulkanDevice {
	public:
		VulkanDevice(VkDevice device, QueueFamilyIndices indices);
		~VulkanDevice();
		VulkanDevice(VulkanDevice&&) = default;
		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice& operator=(VulkanDevice&&) = default;
		VulkanDevice& operator=(const VulkanDevice&) = delete;

	private:
		VkDevice    m_device;
		VulkanQueue m_graphics_queue;
		VulkanQueue m_present_queue;

		VulkanQueue initQueue(int queue_family_index);
	};

}