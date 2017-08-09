#include "device.h"
#include "buffer.h"
#include "helpers.h"

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

        Device::Device(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<const char*> enabled_extensions) :
            Device(physical_device, findQueueFamilies(surface, physical_device), enabled_extensions) {
        }
        
        Device::Device(VkPhysicalDevice physical_device, QueueFamilyIndices indices, const std::vector<const char*> enabled_extensions) : 
            Deleter<VkDevice>(initDevice(physical_device, indices, enabled_extensions), vkDestroyDevice),
            physical_device (physical_device),
            graphics_queue  (*this, indices.graphics_family_index),
            present_queue   (*this, indices.present_family_index) {
            
            // Properties
            vkGetPhysicalDeviceProperties(physical_device, &properties);

            // Queue family properties
            queue_family_properties = helpers::getQueueFamilyProperties(physical_device);
            assert(queue_family_properties.size() > 0);
        }

        VkDevice Device::initDevice(VkPhysicalDevice physical_device, QueueFamilyIndices indices, const std::vector<const char*> enabled_extensions) const {
            std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
            float queue_priority = 1.0f;
            for (auto queue_family_index : {
                indices.graphics_family_index,
                indices.present_family_index
            }) {
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
            create_info.enabledExtensionCount   = enabled_extensions.size();
            create_info.ppEnabledExtensionNames = enabled_extensions.data();
            create_info.enabledLayerCount       = ENABLE_VALIDATION_LAYERS ? g_validation_layers.size() : 0;
            create_info.ppEnabledLayerNames     = ENABLE_VALIDATION_LAYERS ? g_validation_layers.data() : nullptr;

            VkDevice device;
            if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create logical device.");
            }
            return device;
        }
        
        QueueFamilyIndices Device::findQueueFamilies(VkSurfaceKHR surface) const {
            return findQueueFamilies(surface, physical_device);
        }

        QueueFamilyIndices Device::findQueueFamilies(VkSurfaceKHR surface, VkPhysicalDevice physical_device) {
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

        QueueFamilyIndices::operator bool() const {
            return graphics_family_index >= 0 && present_family_index >= 0;
        }

        uint32_t Device::findQueueFamilyIndex(VkQueueFlagBits queue_flags) const {
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

        std::vector<VkImage> Device::getSwapchainImages(VkSwapchainKHR swapchain, uint32_t image_count) const {
            vkGetSwapchainImagesKHR(*this, swapchain, &image_count, nullptr);
            std::vector<VkImage> images{image_count};
            vkGetSwapchainImagesKHR(*this, swapchain, &image_count, images.data());
            return images;
        }

        Deleter<VkSemaphore> Device::createSemaphore() const {
            VkSemaphoreCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkSemaphore semaphore;
            if (vkCreateSemaphore(*this, &create_info, nullptr, &semaphore) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphore.");
            }
            return {semaphore, *this, vkDestroySemaphore};
        }

    }
}