#include "memory.h"

namespace stirling {
    namespace vulkan {

        Memory::Memory() {
        }

        Memory::operator VkDeviceMemory() const {
        }

        void Memory::map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
        }
        
        void Memory::unmap() {
        }

        void Memory::memcpy(const void* data) {
        }

        Deleter<VkDeviceMemory> Memory::allocateMemory(VkMemoryPropertyFlags properties) const {
        }
        
        uint32_t Memory::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
        }

        VkDescriptorBufferInfo Memory::initDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const {
        }

    }
}