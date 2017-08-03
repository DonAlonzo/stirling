#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.h"
#include "memory.h"

namespace stirling {
    namespace vulkan {

        struct Buffer : Deleter<VkBuffer> {
            Memory                  memory;
			VkDescriptorBufferInfo  descriptor;

            Buffer(const Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size);

        private:
            VkBuffer                init(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage) const;
            Memory                  initMemory(const Device& device, VkMemoryPropertyFlags properties) const;
            VkDescriptorBufferInfo  initDescriptor(VkDeviceSize size) const;
        };

    }
}