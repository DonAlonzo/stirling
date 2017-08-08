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

        struct Device {
            const PhysicalDevice& physical_device;
        private:
            Deleter<VkDevice>     device;
        public:
            Queue                 graphics_queue;
            Queue                 present_queue;

            Device(const PhysicalDevice& physical_device, VkDevice device, QueueFamilyIndices indices);

            operator VkDevice() const;

            std::vector<VkImage> getSwapchainImages(VkSwapchainKHR swapchain, uint32_t count) const;
            Deleter<VkSemaphore> createSemaphore() const;

        private:
            Queue                 initQueue(int queue_family_index);
        };
    }
}