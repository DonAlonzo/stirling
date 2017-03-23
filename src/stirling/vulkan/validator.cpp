#include "validator.h"

#include <iostream>
#include <stdexcept>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
	std::cerr << "Validation layer: " << msg << std::endl;
	return VK_FALSE;
}

VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugReportCallbackEXT* callback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if (func != nullptr) {
		return func(instance, create_info, allocator, callback);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* allocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, allocator);
	}
}

namespace stirling {

	VulkanValidator::VulkanValidator(VkInstance instance) :
		m_instance       (instance),
		m_debug_callback (initDebugCallback(instance)) {
	}

	VulkanValidator::VulkanValidator() :
		m_debug_callback (VK_NULL_HANDLE) {
	}

	VulkanValidator VulkanValidator::nullValidator() {
		return VulkanValidator();
	}

	VkDebugReportCallbackEXT VulkanValidator::initDebugCallback(VkInstance instance) const {
		VkDebugReportCallbackCreateInfoEXT create_info = {};
		create_info.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		create_info.flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		create_info.pfnCallback = debugCallback;

		VkDebugReportCallbackEXT debug_callback;
		if (createDebugReportCallbackEXT(instance, &create_info, nullptr, &debug_callback) != VK_SUCCESS) {
			throw std::runtime_error("Failed to set up debug callback.");
		}
		return debug_callback;
	}

	VulkanValidator::~VulkanValidator() {
		if (m_debug_callback != VK_NULL_HANDLE) {
			destroyDebugReportCallbackEXT(m_instance, m_debug_callback, nullptr);
		}
	}

}