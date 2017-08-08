#include "device.h"
#include "buffer.h"
#include "physical_device.h"

#include <vector>

namespace stirling {
    namespace vulkan {

        Device::Device(const PhysicalDevice& physical_device, VkDevice device, QueueFamilyIndices indices) :
            physical_device (physical_device),
            device          (device, vkDestroyDevice),
            graphics_queue  (initQueue(indices.graphics_family_index)),
            present_queue   (initQueue(indices.present_family_index)) {
        }

        Queue Device::initQueue(int queue_family_index) {
            return Queue(*this, queue_family_index);
        }

        Device::operator VkDevice() const {
            return device;
        }

        const PhysicalDevice& Device::getPhysicalDevice() const {
            return physical_device;
        }

        Queue Device::getGraphicsQueue() const {
            return graphics_queue;
        }

        Queue Device::getPresentQueue() const {
            return present_queue;
        }

        std::vector<VkImage> Device::getSwapchainImages(VkSwapchainKHR swapchain, uint32_t image_count) const {
            vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr);
            std::vector<VkImage> images{image_count};
            vkGetSwapchainImagesKHR(device, swapchain, &image_count, images.data());
            return images;
        }

        Deleter<VkSemaphore> Device::createSemaphore() const {
            VkSemaphoreCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkSemaphore semaphore;
            if (vkCreateSemaphore(device, &create_info, nullptr, &semaphore) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphore.");
            }
            return {semaphore, device, vkDestroySemaphore};
        }

    }
}