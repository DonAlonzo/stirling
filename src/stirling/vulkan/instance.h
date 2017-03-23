#pragma once

#include "vulkan/vulkan.h"

#include <vector>

namespace stirling {

class VulkanInstance {
public:
	VulkanInstance(const std::vector<const char*>& extensions);
	~VulkanInstance();
	
	VulkanInstance(const VulkanInstance& rhs) = delete;
	VulkanInstance& operator=(const VulkanInstance& rhs) = delete;

	VulkanInstance(VulkanInstance&& rhs) = default;
	VulkanInstance& operator=(VulkanInstance&& rhs) = default;

	std::vector<VkLayerProperties> getLayerProperties();

private:
	VkInstance               m_instance;
	VkDebugReportCallbackEXT m_debug_callback;

	VkInstance               initInstance(std::vector<const char*> extensions);
	VkDebugReportCallbackEXT initDebugCallback(VkInstance instance);

	bool checkValidationLayerSupport();
};

}