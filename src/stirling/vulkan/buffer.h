#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        class Device;
    }
}

namespace stirling {
    namespace vulkan {
        class Buffer {
            friend class Device;

        public:
            ~Buffer();
            Buffer(Buffer&&);
            Buffer(const Buffer&) = delete;
            Buffer& operator=(Buffer&&);
            Buffer& operator=(const Buffer&) = delete;

            operator VkBuffer() const;

            const VkDeviceMemory& getMemory() const;
            const VkDescriptorBufferInfo& getDescriptor() const;
            const VkDeviceSize& getSize() const;

            void* getMapped() const;
            void map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void unmap();
            void memcpy(const void* data);

        protected:
            const Device*          m_device;
            VkDeviceSize           m_size;
            VkBuffer               m_buffer;
            VkDeviceMemory         m_memory;
            VkDescriptorBufferInfo m_descriptor;
            void*                  m_mapped = nullptr;

            Buffer(const Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size);

        private:
            VkBuffer               initBuffer(VkDeviceSize size, VkBufferUsageFlags usage) const;
            VkDeviceMemory         allocateMemory(VkMemoryPropertyFlags properties) const;
            uint32_t               findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
            VkDescriptorBufferInfo initDescriptor(const VkDeviceSize& size = VK_WHOLE_SIZE, const VkDeviceSize& offset = 0) const;
        };
    }
}