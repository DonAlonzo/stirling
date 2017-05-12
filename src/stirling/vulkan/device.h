#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
namespace stirling {
    namespace vulkan {
        class Buffer;
        class PhysicalDevice;
        class Semaphore;
        struct QueueFamilyIndices;
    }
}
#include "queue.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        class Device {
        public:
            Device(const PhysicalDevice& physical_device, VkDevice device, QueueFamilyIndices indices);
            ~Device();
            Device(Device&&) = default;
            Device(const Device&) = delete;
            Device& operator=(Device&&) = delete;
            Device& operator=(const Device&) = delete;

            operator VkDevice() const;

            const PhysicalDevice& getPhysicalDevice() const;
            std::vector<VkImage> getSwapchainImages(VkSwapchainKHR swapchain, uint32_t count) const;
            Queue getGraphicsQueue() const;
            Queue getPresentQueue() const;

            Buffer createBuffer(VkBufferUsageFlags usage_flags, VkMemoryPropertyFlags memory_property_flags, VkDeviceSize size, void* data = nullptr) const;
            Semaphore createSemaphore() const;

        private:
            const PhysicalDevice& m_physical_device;

            VkDevice              m_device;
            Queue                 m_graphics_queue;
            Queue                 m_present_queue;

            Queue                 initQueue(int queue_family_index);
        };
    }
}