#pragma once

#include "vulkan/vulkan.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        namespace helpers {

            inline VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
                VkSurfaceCapabilitiesKHR capabilities;
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);
                return capabilities;
            }

            inline std::vector<VkSurfaceFormatKHR> getSurfaceFormats(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
                uint32_t format_count;
                vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
                std::vector<VkSurfaceFormatKHR> formats{ format_count };
                vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats.data());
                return formats;
            }

            inline std::vector<VkPresentModeKHR> getSurfacePresentModes(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
                uint32_t present_mode_count;
                vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
                std::vector<VkPresentModeKHR> present_modes{ present_mode_count };
                vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data());
                return present_modes;
            }

            inline std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice physical_device) {
                uint32_t queue_family_count;
                vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
                std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
                vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_properties.data());
                return queue_family_properties;
            }

            inline std::vector<VkExtensionProperties> getExtensions(VkPhysicalDevice physical_device) {
                uint32_t extension_count;
                vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);
                std::vector<VkExtensionProperties> extensions(extension_count);
                vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, extensions.data());
                return extensions;
            }

        }
    }
}