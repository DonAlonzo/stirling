#include "buffer.h"
#include "device.h"
#include "physical_device.h"

#include <cstring>

namespace stirling {
    namespace vulkan {

        Buffer::Buffer(const Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size) :
            m_device     (&device),
            m_size       (size),
            m_buffer     (initBuffer(size, usage)),
            m_memory     (allocateMemory(properties)),
            m_descriptor (initDescriptor(size)) {

            vkBindBufferMemory(device, m_buffer, m_memory, 0);
        }

        VkBuffer Buffer::initBuffer(VkDeviceSize size, VkBufferUsageFlags usage) const {
            VkBufferCreateInfo create_info = {};
            create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            create_info.size        = size;
            create_info.usage       = usage;
            create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VkBuffer buffer;
            if (vkCreateBuffer(*m_device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create buffer.");
            }
            return buffer;
        }

        VkDeviceMemory Buffer::allocateMemory(VkMemoryPropertyFlags properties) const {
            VkMemoryRequirements memory_requirements;
            vkGetBufferMemoryRequirements(*m_device, m_buffer, &memory_requirements);

            VkMemoryAllocateInfo allocate_info = {};
            allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocate_info.allocationSize  = memory_requirements.size;
            allocate_info.memoryTypeIndex = findMemoryType(memory_requirements.memoryTypeBits, properties);

            VkDeviceMemory memory;
            if (vkAllocateMemory(*m_device, &allocate_info, nullptr, &memory) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate buffer memory.");
            }
            return memory;
        }

        uint32_t Buffer::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
            VkPhysicalDeviceMemoryProperties memory_properties;
            vkGetPhysicalDeviceMemoryProperties(m_device->getPhysicalDevice(), &memory_properties);

            for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i) {
                if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }
            throw std::runtime_error("Failed to find suitable memory type.");
        }

        VkDescriptorBufferInfo Buffer::initDescriptor(const VkDeviceSize& size, const VkDeviceSize& offset) const {
            VkDescriptorBufferInfo descriptor = {};
            descriptor.buffer = m_buffer;
            descriptor.range  = size;
            descriptor.offset = offset;
            return descriptor;
        }

        Buffer::Buffer(Buffer&& rhs) :
            m_device(std::move(rhs.m_device)),
            m_buffer(std::move(rhs.m_buffer)),
            m_memory(std::move(rhs.m_memory)) {

            rhs.m_buffer = VK_NULL_HANDLE;
            rhs.m_memory = VK_NULL_HANDLE;
        }

        Buffer& Buffer::operator=(Buffer&& rhs) {
            if (m_memory != VK_NULL_HANDLE) vkFreeMemory(*m_device, m_memory, nullptr);
            if (m_buffer != VK_NULL_HANDLE) vkDestroyBuffer(*m_device, m_buffer, nullptr);

            m_device = std::move(rhs.m_device);
            m_buffer = std::move(rhs.m_buffer);
            m_memory = std::move(rhs.m_memory);

            rhs.m_buffer = VK_NULL_HANDLE;
            rhs.m_memory = VK_NULL_HANDLE;

            return *this;
        }

        Buffer::~Buffer() {
            if (m_memory != VK_NULL_HANDLE) vkFreeMemory(*m_device, m_memory, nullptr);
            if (m_buffer != VK_NULL_HANDLE) vkDestroyBuffer(*m_device, m_buffer, nullptr);
        }

        Buffer::operator VkBuffer() const {
            return m_buffer;
        }

        void Buffer::map(VkDeviceSize size, VkDeviceSize offset) {
            if (m_mapped) throw std::runtime_error("Buffer memory has already been mapped.");

            if (vkMapMemory(*m_device, m_memory, offset, size, 0, &m_mapped) != VK_SUCCESS) {
                throw std::runtime_error("Could not map buffer memory.");
            }
        }

        void Buffer::unmap() {
            if (!m_mapped) throw std::runtime_error("Buffer memory has not been mapped yet.");

            vkUnmapMemory(*m_device, m_memory);
            m_mapped = nullptr;
        }

        void Buffer::memcpy(const void* data) {
            if (!m_mapped) throw std::runtime_error("Buffer memory has not been mapped yet.");

            std::memcpy(m_mapped, data, m_size);
        }

    }
}