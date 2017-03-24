#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {

	class VulkanValidator {
	public:
		VulkanValidator(VkInstance instance);
		~VulkanValidator();
		VulkanValidator(VulkanValidator&&) = default;
		VulkanValidator(const VulkanValidator&) = delete;
		VulkanValidator& operator=(VulkanValidator&&) = default;
		VulkanValidator& operator=(const VulkanValidator&) = delete;

		static VulkanValidator nullValidator();

	private:
		VkInstance               m_instance;
		VkDebugReportCallbackEXT m_debug_callback;

		VulkanValidator();

		VkDebugReportCallbackEXT initDebugCallback(VkInstance instance) const;
	};
	
}