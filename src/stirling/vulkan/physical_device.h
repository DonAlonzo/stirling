#pragma once

#include "vulkan/vulkan.h"

namespace stirling {

	class VulkanPhysicalDevice {
	public:
		VulkanPhysicalDevice(VkPhysicalDevice physical_device);
		
		operator VkPhysicalDevice() const;

		bool hasAllQueueFamilies() const;
		int getGraphicsFamilyIndex() const;

	private:
		VkPhysicalDevice m_physical_device;
		int              m_graphics_family_index;

		void initQueueFamilyIndices();
	};

}