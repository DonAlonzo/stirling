#include "memory.h"

#include <cstring>
#include <stdexcept>

namespace stirling {
    namespace vulkan {
        
        Memory::Memory(const Device& device, VkMemoryRequirements memory_requirements, VkMemoryPropertyFlags properties) :
            Deleter<VkDeviceMemory>(allocate(device, memory_requirements, properties), device, vkFreeMemory),
            device (device),
            size   (memory_requirements.size) {
        }

        VkDeviceMemory Memory::allocate(const Device& device, VkMemoryRequirements memory_requirements, VkMemoryPropertyFlags properties) const {
            VkMemoryAllocateInfo allocate_info = {};
            allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocate_info.allocationSize  = memory_requirements.size;
            allocate_info.memoryTypeIndex = findMemoryType(device.getPhysicalDevice(), memory_requirements.memoryTypeBits, properties);

            VkDeviceMemory memory;
            if (vkAllocateMemory(device, &allocate_info, nullptr, &memory) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate buffer memory.");
            }
            return memory;
        }
        
        uint32_t Memory::findMemoryType(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties) const {
            VkPhysicalDeviceMemoryProperties memory_properties;
            vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

            for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i) {
                if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }
            throw std::runtime_error("Failed to find suitable memory type.");
        }
        
		MemoryMapping Memory::map(VkDeviceSize size, VkDeviceSize offset) const {
			return MemoryMapping(device, *this, size, offset);
        }

		MemoryMapping::MemoryMapping(VkDevice device, VkDeviceMemory memory, VkDeviceSize size, VkDeviceSize offset) :
			device (device),
			memory (memory) {

			if (vkMapMemory(device, memory, offset, size, 0, &mapped) != VK_SUCCESS) {
				throw std::runtime_error("Could not map buffer memory.");
			}
		}

		MemoryMapping::~MemoryMapping() {
			vkUnmapMemory(device, memory);
		}

		void MemoryMapping::memcpy(const void* data, size_t count) const {
			std::memcpy(mapped, data, count);
		}
        
    }
}