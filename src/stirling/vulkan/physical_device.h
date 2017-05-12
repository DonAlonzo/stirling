#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
namespace stirling {
    namespace vulkan {
        class Surface;
    }
}
#include "device.h"

// std
#include <vector>

namespace stirling {
    namespace vulkan {

        struct QueueFamilyIndices {
            int graphics_family_index;
            int present_family_index;

            bool isComplete() const;
        };

        class PhysicalDevice {
        public:
            std::vector<VkExtensionProperties>   extensions;
            VkPhysicalDeviceProperties           properties;
            std::vector<VkQueueFamilyProperties> queue_family_properties;

            PhysicalDevice(VkPhysicalDevice physical_device);
        
            operator VkPhysicalDevice() const;

            uint32_t findQueueFamilyIndex(VkQueueFlagBits queue_flags) const;
            QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface) const;

            Device createDevice(VkSurfaceKHR surface, const std::vector<const char*> extensions) const;

            VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkSurfaceKHR surface) const;
            std::vector<VkSurfaceFormatKHR> getSurfaceFormats(VkSurfaceKHR surface) const;
            std::vector<VkPresentModeKHR> getSurfacePresentModes(VkSurfaceKHR surface) const;

        private:
            VkPhysicalDevice physical_device;
        };

    }
}