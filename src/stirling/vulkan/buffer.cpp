#include "buffer.h"

#include "device.h"
#include "physical_device.h"

#include <cstring>

namespace stirling {
    namespace vulkan {

        Buffer::Buffer(const Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size) :
            Deleter<VkBuffer>(init(device, size, usage), device, vkDestroyBuffer),
            memory     (initMemory(device, properties)),
            descriptor (initDescriptor(size)) {

            vkBindBufferMemory(device, *this, memory, 0);
        }

        VkBuffer Buffer::init(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage) const {
            VkBufferCreateInfo create_info = {};
            create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            create_info.size        = size;
            create_info.usage       = usage;
            create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VkBuffer buffer;
            if (vkCreateBuffer(device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create buffer.");
            }
            return buffer;
        }

        Memory Buffer::initMemory(const Device& device, VkMemoryPropertyFlags properties) const {
            VkMemoryRequirements memory_requirements;
            vkGetBufferMemoryRequirements(device, *this, &memory_requirements);
            return {device, memory_requirements, properties};
        }

        VkDescriptorBufferInfo Buffer::initDescriptor(VkDeviceSize size) const {
            VkDescriptorBufferInfo descriptor = {};
            descriptor.buffer = *this;
            descriptor.range  = size;
            descriptor.offset = 0;
            return descriptor;
        }

    }
}