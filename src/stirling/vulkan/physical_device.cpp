#include "physical_device.h"
#include "device.h"

#include <assert.h>
#include <vector>
#include <set>

#ifdef NDEBUG
constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> g_validation_layers = {
    "VK_LAYER_LUNARG_standard_validation"
};

namespace stirling {
    namespace vulkan {
    
        PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device) {
            this->physical_device = physical_device;

            // Properties
            vkGetPhysicalDeviceProperties(physical_device, &properties);

            // Extensions
            uint32_t extension_count;
            vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);
            extensions.resize(extension_count);
            vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, extensions.data());

            // Queue family properties
            uint32_t queue_family_count;
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
            assert(queue_family_count > 0);
            queue_family_properties.resize(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_properties.data());
        }

        PhysicalDevice::operator VkPhysicalDevice() const {
            return physical_device;
        }

        QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkSurfaceKHR surface) const {
            uint32_t queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
            std::vector<VkQueueFamilyProperties> queue_families{ queue_family_count };
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

            QueueFamilyIndices indices = {};
            for (int i = 0; i < queue_families.size(); ++i) {
                if (queue_families[i].queueCount == 0) continue;

                if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    indices.graphics_family_index = i;
                }

                VkBool32 present_support = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);

                if (queue_families[i].queueCount > 0 && present_support) {
                    indices.present_family_index = i;
                }

                if (indices) break;
            }
            return indices;
        }

        Device PhysicalDevice::createDevice(VkSurfaceKHR surface, const std::vector<const char*> extensions) const {
            QueueFamilyIndices indices = findQueueFamilies(surface);

            std::set<int> queue_family_indices = {
                indices.graphics_family_index,
                indices.present_family_index
            };

            std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
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
            device_features.samplerAnisotropy = VK_TRUE;

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
            if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create logical device.");
            }
            return Device(*this, device, indices);
        }

        VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(VkSurfaceKHR surface) const {
            VkSurfaceCapabilitiesKHR capabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);
            return capabilities;
        }


        std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(VkSurfaceKHR surface) const {
            uint32_t format_count;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
            std::vector<VkSurfaceFormatKHR> formats{format_count};
            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats.data());
            return formats;
        }

        std::vector<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(VkSurfaceKHR surface) const {
            uint32_t present_mode_count;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
            std::vector<VkPresentModeKHR> present_modes{present_mode_count};
            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data());
            return present_modes;
        }

        QueueFamilyIndices::operator bool() const {
            return graphics_family_index >= 0 && present_family_index >= 0;
        }

        uint32_t PhysicalDevice::findQueueFamilyIndex(VkQueueFlagBits queue_flags) const {
            if (queue_flags & VK_QUEUE_COMPUTE_BIT) {
                for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i) {
                    if ((queue_family_properties[i].queueFlags & queue_flags) &&
                        (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) {
                        return i;
                    }
                }
            }

            if (queue_flags & VK_QUEUE_TRANSFER_BIT) {
                for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i) {
                    if ((queue_family_properties[i].queueFlags & queue_flags) &&
                       ((queue_family_properties[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) == 0)) {
                        return i;
                    }
                }
            }

            for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i) {
                if (queue_family_properties[i].queueFlags & queue_flags) {
                    return i;
                }
            }

            throw std::runtime_error("Could not find queue family index.");
        }

    }
}