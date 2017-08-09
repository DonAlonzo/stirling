#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
namespace stirling {
    namespace vulkan {
        struct Buffer;
        struct PhysicalDevice;
        struct QueueFamilyIndices;
    }
}
#include "deleter.h"
#include "queue.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        
        struct QueueFamilyIndices {
            int graphics_family_index;
            int present_family_index;

            operator bool() const;
        };

        struct Device : Deleter<VkDevice> {
            VkPhysicalDeviceProperties           properties;
            std::vector<VkQueueFamilyProperties> queue_family_properties;
            VkPhysicalDevice                     physical_device;
            Queue                                graphics_queue;
            Queue                                present_queue;

            Device(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<const char*> enabled_extensions);
            
            uint32_t             findQueueFamilyIndex(VkQueueFlagBits queue_flags) const;
            QueueFamilyIndices   findQueueFamilies(VkSurfaceKHR surface) const;
            std::vector<VkImage> getSwapchainImages(VkSwapchainKHR swapchain, uint32_t count) const;
            Deleter<VkSemaphore> createSemaphore() const;

            static QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface, VkPhysicalDevice physical_device);

        private:
            Device(VkPhysicalDevice physical_device, QueueFamilyIndices indices, const std::vector<const char*> enabled_extensions);
            
            VkDevice initDevice(VkPhysicalDevice physical_device, QueueFamilyIndices indices, const std::vector<const char*> extensions) const;
        };
    }
}