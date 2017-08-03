#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.h"
#include "memory.h"

namespace stirling {
    namespace vulkan {

        class Buffer {
            friend class Device;
            
			Deleter<VkBuffer>       buffer;
        public:
            Memory                  memory;
			VkDescriptorBufferInfo  descriptor;

            Buffer(const Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size);

            operator VkBuffer() const;

            MemoryMapping map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

        private:
            Deleter<VkBuffer>       init(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage) const;
            Memory                  initMemory(const Device& device, VkMemoryPropertyFlags properties) const;
            VkDescriptorBufferInfo  initDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        };
    }
}