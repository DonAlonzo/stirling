#include "buffer.h"

#include "device.h"
#include "physical_device.h"

#include <cstring>

namespace stirling {
    namespace vulkan {

        Buffer::Buffer(const Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size) :
            buffer     (init(device, size, usage)),
            memory     (initMemory(device, properties)),
            descriptor (initDescriptor(size)) {

            vkBindBufferMemory(device, buffer, memory, 0);
        }

        Deleter<VkBuffer> Buffer::init(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage) const {
            VkBufferCreateInfo create_info = {};
            create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            create_info.size        = size;
            create_info.usage       = usage;
            create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VkBuffer buffer;
            if (vkCreateBuffer(device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create buffer.");
            }
            return Deleter<VkBuffer>(buffer, device, vkDestroyBuffer);
        }

        Memory Buffer::initMemory(const Device& device, VkMemoryPropertyFlags properties) const {
            VkMemoryRequirements memory_requirements;
            vkGetBufferMemoryRequirements(device, buffer, &memory_requirements);
            return {device, memory_requirements, properties};
        }

        VkDescriptorBufferInfo Buffer::initDescriptor(VkDeviceSize size, VkDeviceSize offset) const {
            VkDescriptorBufferInfo descriptor = {};
            descriptor.buffer = buffer;
            descriptor.range  = size;
            descriptor.offset = offset;
            return descriptor;
        }

        Buffer::operator VkBuffer() const {
            return buffer;
        }
        
		MemoryMapping Buffer::map(VkDeviceSize size, VkDeviceSize offset) const {
			return memory.map(size, offset);
        }

    }
}