#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.h"

namespace stirling {
    namespace vulkan {

		struct BufferMapping {
			VkDevice       device;
			VkDeviceMemory memory;
			void*          mapped = nullptr;

			BufferMapping(VkDevice device, VkDeviceMemory memory, VkDeviceSize size, VkDeviceSize offset);
			~BufferMapping();
			BufferMapping(BufferMapping&&) = default;
			BufferMapping& operator=(BufferMapping&&) = default;

			void memcpy(const void* data, size_t count) const;
		};

        class Buffer {
            friend class Device;

        private:
            const Device* device;

        public:
			VkDeviceSize            size;
			Deleter<VkBuffer>       buffer;
			Deleter<VkDeviceMemory> memory;
			VkDescriptorBufferInfo  descriptor;

            Buffer(const Device* device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size);

            operator VkBuffer() const;

            BufferMapping map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

        private:
            Deleter<VkBuffer>       initBuffer(VkDeviceSize size, VkBufferUsageFlags usage) const;
            Deleter<VkDeviceMemory> allocateMemory(VkMemoryPropertyFlags properties) const;
            uint32_t                findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
            VkDescriptorBufferInfo  initDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        };
    }
}