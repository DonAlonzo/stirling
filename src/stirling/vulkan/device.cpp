#include "device.h"
#include "physical_device.h"

#include <vector>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> g_validation_layers = {
	"VK_LAYER_LUNARG_standard_validation"
};

namespace stirling {

	VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& physical_device) :
		m_device         (initDevice(physical_device)) {

	}

	VkDevice VulkanDevice::initDevice(const VulkanPhysicalDevice& physical_device) {
		float queue_priority = 1.0f;

		VkDeviceQueueCreateInfo queue_create_info = {};
		queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = physical_device.getGraphicsFamilyIndex();
		queue_create_info.queueCount       = 1;
		queue_create_info.pQueuePriorities = &queue_priority;

		VkPhysicalDeviceFeatures device_features = {};

		VkDeviceCreateInfo create_info = {};
		create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.pQueueCreateInfos       = &queue_create_info;
		create_info.queueCreateInfoCount    = 1;
		create_info.pEnabledFeatures        = &device_features;
		create_info.enabledExtensionCount   = 0;
		create_info.ppEnabledExtensionNames = nullptr;
		create_info.enabledLayerCount       = ENABLE_VALIDATION_LAYERS ? g_validation_layers.size() : 0;
		create_info.ppEnabledLayerNames     = ENABLE_VALIDATION_LAYERS ? g_validation_layers.data() : nullptr;

		VkDevice device;
		if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create logical device.");
		}
		return device;
	}

	VulkanDevice::~VulkanDevice() {
		vkDestroyDevice(m_device, nullptr);
	}

}