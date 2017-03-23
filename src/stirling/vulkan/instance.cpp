#include "instance.h"

#include <stdexcept>
#include <iostream>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> g_validation_layers = {
	"VK_LAYER_LUNARG_standard_validation"
};

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

VulkanInstance::VulkanInstance(const std::vector<const char*>& extensions) :
	m_instance       (initInstance(extensions)),
	m_debug_callback (initDebugCallback(m_instance)) {
}

VkInstance VulkanInstance::initInstance(std::vector<const char*> extensions) {
	if (ENABLE_VALIDATION_LAYERS) {
		if (!checkValidationLayerSupport()) {
			throw std::runtime_error("Validation layers requested, but not available.");
		}
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	VkApplicationInfo application_info = {};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pApplicationName = "Stirling Engine Demo";
	application_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	application_info.pEngineName = "Stirling Engine";
	application_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	application_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &application_info;
	create_info.enabledExtensionCount = extensions.size();
	create_info.ppEnabledExtensionNames = extensions.data();
	create_info.enabledLayerCount = ENABLE_VALIDATION_LAYERS ? g_validation_layers.size() : 0;
	create_info.ppEnabledLayerNames = ENABLE_VALIDATION_LAYERS ? g_validation_layers.data() : nullptr;

	VkInstance instance;
	if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance.");
	}
	return instance;
}

VkDebugReportCallbackEXT VulkanInstance::initDebugCallback(VkInstance instance) {
	if (!ENABLE_VALIDATION_LAYERS) return VK_NULL_HANDLE;

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

VulkanInstance::~VulkanInstance() {
	destroyDebugReportCallbackEXT(m_instance, m_debug_callback, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

bool VulkanInstance::checkValidationLayerSupport() {
	auto available_layers = getLayerProperties();

	for (auto validation_layer : g_validation_layers) {
		bool found = false;

		for (const auto& available_layer : available_layers) {
			if (strcmp(validation_layer, available_layer.layerName) == 0) {
				found = true;
				break;
			}
		}

		if (!found) return false;
	}

	return true;
}

std::vector<VkLayerProperties> VulkanInstance::getLayerProperties() {
	uint32_t instance_layer_count;
	vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr);
	std::vector<VkLayerProperties> instance_layers{ instance_layer_count };
	vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layers.data());
	return instance_layers;
}

}