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

private:
	VkInstance m_instance;
};

}