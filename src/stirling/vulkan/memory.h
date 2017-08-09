#pragma once

#include "vulkan/vulkan.h"

#include "deleter.h"
#include "device.h"

namespace stirling {
    namespace vulkan {

		struct MemoryMapping {
			VkDevice       device;
			VkDeviceMemory memory;
			void*          mapped = nullptr;

			MemoryMapping(VkDevice device, VkDeviceMemory memory, VkDeviceSize size, VkDeviceSize offset);
			~MemoryMapping();
			MemoryMapping(MemoryMapping&&) = default;
			MemoryMapping& operator=(MemoryMapping&&) = default;

			void memcpy(const void* data, size_t count) const;
		};

        struct Memory : Deleter<VkDeviceMemory> {
            VkDeviceSize size;

            Memory(const Device& device, VkMemoryRequirements memory_requirements, VkMemoryPropertyFlags properties);

            MemoryMapping map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

        private:
			VkDevice device;
            
            VkDeviceMemory allocate(const Device& device, VkMemoryRequirements memory_requirements, VkMemoryPropertyFlags properties) const;
            uint32_t       findMemoryType(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties) const;
        };

    }
}