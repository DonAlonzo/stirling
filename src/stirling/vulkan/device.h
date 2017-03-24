#pragma once

#include "vulkan/vulkan.h"

namespace stirling {

	class QueueFamilyIndices;
	class VulkanPhysicalDevice;

	class VulkanDevice {
	public:
		VulkanDevice(const VulkanPhysicalDevice& physical_device, const QueueFamilyIndices& indices);
		~VulkanDevice();
		VulkanDevice(VulkanDevice&&) = default;
		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice& operator=(VulkanDevice&&) = default;
		VulkanDevice& operator=(const VulkanDevice&) = delete;

	private:
		VkDevice m_device;

		VkDevice initDevice(const VulkanPhysicalDevice& physical_device, const QueueFamilyIndices& indices);
	};

}