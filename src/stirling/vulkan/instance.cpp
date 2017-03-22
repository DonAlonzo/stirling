#include "instance.h"

#include <stdexcept>

namespace stirling {

VulkanInstance::VulkanInstance(const std::vector<const char*>& extensions) {
	VkApplicationInfo application_info = {};
	application_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pApplicationName   = "Stirling Engine Demo";
	application_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	application_info.pEngineName        = "Stirling Engine";
	application_info.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
	application_info.apiVersion         = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo        = &application_info;
	createInfo.enabledExtensionCount   = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount       = 0;

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance.");
	}
}

VulkanInstance::~VulkanInstance() {
	vkDestroyInstance(m_instance, nullptr);
}

}