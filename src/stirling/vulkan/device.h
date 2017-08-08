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
            Device(const PhysicalDevice& physical_device, VkDevice device, QueueFamilyIndices indices);

            operator VkDevice() const;

            const PhysicalDevice& getPhysicalDevice() const;
            std::vector<VkImage> getSwapchainImages(VkSwapchainKHR swapchain, uint32_t count) const;
            Queue getGraphicsQueue() const;
            Queue getPresentQueue() const;

            Deleter<VkSemaphore> createSemaphore() const;

        private:
            const PhysicalDevice& physical_device;

            Deleter<VkDevice>     device;
            Queue                 graphics_queue;
            Queue                 present_queue;

            Queue                 initQueue(int queue_family_index);
        };
    }
}