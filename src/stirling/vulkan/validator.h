#pragma once

#include "vulkan/vulkan.h"

namespace stirling {

	class VulkanValidator {
	public:
		VulkanValidator(VkInstance instance);
		~VulkanValidator();

		static VulkanValidator nullValidator();

		VulkanValidator(const VulkanValidator& rhs) = delete;
		VulkanValidator& operator=(const VulkanValidator& rhs) = delete;

		VulkanValidator(VulkanValidator&& rhs) = default;
		VulkanValidator& operator=(VulkanValidator && rhs) = default;

	private:
		VkInstance               m_instance;
		VkDebugReportCallbackEXT m_debug_callback;

		VulkanValidator();

		VkDebugReportCallbackEXT initDebugCallback(VkInstance instance) const;
	};
	
}