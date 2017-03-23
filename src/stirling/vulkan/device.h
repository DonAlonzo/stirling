#pragma once

#include "vulkan/vulkan.h"

namespace stirling {

	class VulkanPhysicalDevice;

	class VulkanDevice {
	public:
		VulkanDevice(const VulkanPhysicalDevice& physical_device);
		~VulkanDevice();

		VulkanDevice(const VulkanDevice& rhs) = delete;
		VulkanDevice& operator=(const VulkanDevice& rhs) = delete;

		VulkanDevice(VulkanDevice&& rhs) = default;
		VulkanDevice& operator=(VulkanDevice&& rhs) = default;

	private:
		VkDevice m_device;

		VkDevice initDevice(const VulkanPhysicalDevice& physical_device);
	};

}