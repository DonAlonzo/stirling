#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.h"

namespace stirling {
    namespace vulkan {
        class Buffer {
            friend class Device;

        private:
            const Device& m_device;

        public:
            Buffer(const Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size);

            operator VkBuffer() const;

            void map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void unmap();
            void memcpy(const void* data);

            VkDeviceSize            m_size;
            Deleter<VkBuffer>       m_buffer;
            Deleter<VkDeviceMemory> m_memory;
            VkDescriptorBufferInfo  m_descriptor;
            void*                   m_mapped = nullptr;

        private:
            Deleter<VkBuffer>       initBuffer(VkDeviceSize size, VkBufferUsageFlags usage) const;
            Deleter<VkDeviceMemory> allocateMemory(VkMemoryPropertyFlags properties) const;
            uint32_t                findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
            VkDescriptorBufferInfo  initDescriptor(const VkDeviceSize& size = VK_WHOLE_SIZE, const VkDeviceSize& offset = 0) const;
        };
    }
}