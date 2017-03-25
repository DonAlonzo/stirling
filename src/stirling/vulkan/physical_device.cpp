#include "physical_device.h"
#include "device.h"
#include "surface.h"

#include <vector>
#include <set>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> g_validation_layers = {
	"VK_LAYER_LUNARG_standard_validation"
};

namespace stirling {
	namespace vulkan {
	
		PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device) :
			m_physical_device (physical_device) {
		}

		PhysicalDevice::operator VkPhysicalDevice() const {
			return m_physical_device;
		}

		QueueFamilyIndices PhysicalDevice::findQueueFamilies(const Surface& surface) const {
			uint32_t queue_family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);
			std::vector<VkQueueFamilyProperties> queue_families{ queue_family_count };
			vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_families.data());

			QueueFamilyIndices indices;
			for (int i = 0; i < queue_families.size(); ++i) {
				if (queue_families[i].queueCount == 0) continue;

				if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphics_family_index = i;
				}

				VkBool32 present_support = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, surface, &present_support);

				if (queue_families[i].queueCount > 0 && present_support) {
					indices.present_family_index = i;
				}

				if (indices.isComplete()) break;
			}
			return indices;
		}

		Device PhysicalDevice::createDevice(const Surface& surface, const std::vector<const char*> extensions) const {
			QueueFamilyIndices indices = findQueueFamilies(surface);

			std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
			std::set<int> queue_family_indices = {
				indices.graphics_family_index,
				indices.present_family_index
			};

			float queue_priority = 1.0f;
			for (int queue_family_index : queue_family_indices) {
				VkDeviceQueueCreateInfo queue_create_info = {};
				queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_create_info.queueFamilyIndex = queue_family_index;
				queue_create_info.queueCount       = 1;
				queue_create_info.pQueuePriorities = &queue_priority;
				queue_create_infos.push_back(queue_create_info);
			}

			VkPhysicalDeviceFeatures device_features = {};

			VkDeviceCreateInfo create_info = {};
			create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			create_info.pQueueCreateInfos       = queue_create_infos.data();
			create_info.queueCreateInfoCount    = queue_create_infos.size();
			create_info.pEnabledFeatures        = &device_features;
			create_info.enabledExtensionCount   = extensions.size();
			create_info.ppEnabledExtensionNames = extensions.data();
			create_info.enabledLayerCount       = ENABLE_VALIDATION_LAYERS ? g_validation_layers.size() : 0;
			create_info.ppEnabledLayerNames     = ENABLE_VALIDATION_LAYERS ? g_validation_layers.data() : nullptr;

			VkDevice device;
			if (vkCreateDevice(m_physical_device, &create_info, nullptr, &device) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create logical device.");
			}

			return Device(*this, device, indices);
		}

		std::vector<VkExtensionProperties> PhysicalDevice::getExtensions() const {
			uint32_t extension_count;
			vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &extension_count, nullptr);
			std::vector<VkExtensionProperties> extensions{extension_count};
			vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &extension_count, extensions.data());
			return extensions;
		}

		VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(const Surface& surface) const {
			VkSurfaceCapabilitiesKHR capabilities;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, surface, &capabilities);
			return capabilities;
		}


		std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(const Surface& surface) const {
			uint32_t format_count;
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, nullptr);
			std::vector<VkSurfaceFormatKHR> formats{format_count};
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, formats.data());
			return formats;
		}

		std::vector<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(const Surface& surface) const {
			uint32_t present_mode_count;
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &present_mode_count, nullptr);
			std::vector<VkPresentModeKHR> present_modes{present_mode_count};
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &present_mode_count, present_modes.data());
			return present_modes;
		}

		bool QueueFamilyIndices::isComplete() const {
			return graphics_family_index >= 0
				&& present_family_index >= 0;
		}

	}
}