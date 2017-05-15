#include "device.h"
#include "buffer.h"
#include "physical_device.h"
#include "semaphore.h"

#include <vector>

namespace stirling {
    namespace vulkan {

        Device::Device(const PhysicalDevice& physical_device, VkDevice device, QueueFamilyIndices indices) :
            m_physical_device (physical_device),
            m_device          (device, vkDestroyDevice),
            m_graphics_queue  (initQueue(indices.graphics_family_index)),
            m_present_queue   (initQueue(indices.present_family_index)) {
        }

        Queue Device::initQueue(int queue_family_index) {
            return Queue(*this, queue_family_index);
        }

        Device::operator VkDevice() const {
            return m_device;
        }

        const PhysicalDevice& Device::getPhysicalDevice() const {
            return m_physical_device;
        }

        Queue Device::getGraphicsQueue() const {
            return m_graphics_queue;
        }

        Queue Device::getPresentQueue() const {
            return m_present_queue;
        }

        std::vector<VkImage> Device::getSwapchainImages(VkSwapchainKHR swapchain, uint32_t image_count) const {
            vkGetSwapchainImagesKHR(m_device, swapchain, &image_count, nullptr);
            std::vector<VkImage> images{image_count};
            vkGetSwapchainImagesKHR(m_device, swapchain, &image_count, images.data());
            return images;
        }

        Buffer Device::createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size, void* data) const {
            return Buffer(this, usage, properties, size);
        }

        Semaphore Device::createSemaphore() const {
            return Semaphore(*this);
        }

    }
}