#include "buffer.h"

#include "device.h"
#include "physical_device.h"

#include <cstring>

namespace stirling {
    namespace vulkan {

        Buffer::Buffer(const Device* device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size) :
            device     (device),
            size       (size),
            buffer     (initBuffer(size, usage)),
            memory     (allocateMemory(properties)),
            descriptor (initDescriptor(size)) {

            vkBindBufferMemory(*device, buffer, memory, 0);
        }

        Deleter<VkBuffer> Buffer::initBuffer(VkDeviceSize size, VkBufferUsageFlags usage) const {
            VkBufferCreateInfo create_info = {};
            create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            create_info.size        = size;
            create_info.usage       = usage;
            create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VkBuffer buffer;
            if (vkCreateBuffer(*device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create buffer.");
            }
            return Deleter<VkBuffer>(buffer, *device, vkDestroyBuffer);
        }

        Deleter<VkDeviceMemory> Buffer::allocateMemory(VkMemoryPropertyFlags properties) const {
            VkMemoryRequirements memory_requirements;
            vkGetBufferMemoryRequirements(*device, buffer, &memory_requirements);

            VkMemoryAllocateInfo allocate_info = {};
            allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocate_info.allocationSize  = memory_requirements.size;
            allocate_info.memoryTypeIndex = findMemoryType(memory_requirements.memoryTypeBits, properties);

            VkDeviceMemory memory;
            if (vkAllocateMemory(*device, &allocate_info, nullptr, &memory) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate buffer memory.");
            }
            return Deleter<VkDeviceMemory>(memory, *device, vkFreeMemory);
        }

        uint32_t Buffer::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
            VkPhysicalDeviceMemoryProperties memory_properties;
            vkGetPhysicalDeviceMemoryProperties(device->getPhysicalDevice(), &memory_properties);

            for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i) {
                if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }
            throw std::runtime_error("Failed to find suitable memory type.");
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

		BufferMapping Buffer::map(VkDeviceSize size, VkDeviceSize offset) const {
			return BufferMapping(*device, memory, size, offset);
        }

		BufferMapping::BufferMapping(VkDevice device, VkDeviceMemory memory, VkDeviceSize size, VkDeviceSize offset) :
			device (device),
			memory (memory) {

			if (vkMapMemory(device, memory, offset, size, 0, &mapped) != VK_SUCCESS) {
				throw std::runtime_error("Could not map buffer memory.");
			}
		}

		BufferMapping::~BufferMapping() {
			vkUnmapMemory(device, memory);
		}

		void BufferMapping::memcpy(const void* data, size_t count) const {
			std::memcpy(mapped, data, count);
		}

    }
}