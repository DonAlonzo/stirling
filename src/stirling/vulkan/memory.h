#pragma once

#include "vulkan/vulkan.h"

#include "deleter.h"

namespace stirling {
    namespace vulkan {
        class Memory {
        public:
            Memory();

            operator VkDeviceMemory() const;

            void map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void unmap();
            void memcpy(const void* data);

            VkDeviceSize            size;
            VkDescriptorBufferInfo  descriptor;
            void*                   mapped = nullptr;

        private:
            Deleter<VkDeviceMemory> m_memory;

            Deleter<VkDeviceMemory> allocateMemory(VkMemoryPropertyFlags properties) const;
            uint32_t                findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
            VkDescriptorBufferInfo  initDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        };
    }
}