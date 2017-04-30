#include "command_pool.h"
#include "device.h"

namespace stirling {
    namespace vulkan {

        CommandPool::CommandPool(Device* device, int queue_family_index) :
            m_device       (device),
            m_command_pool (initCommandPool(queue_family_index)) {
        }

        Deleter<VkCommandPool> CommandPool::initCommandPool(int queue_family_index) const {
            VkCommandPoolCreateInfo create_info = {};
            create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            create_info.queueFamilyIndex = queue_family_index;
            create_info.flags            = 0;

            VkCommandPool command_pool;
            if (vkCreateCommandPool(*m_device, &create_info, nullptr, &command_pool) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create command pool.");
            }
            return Deleter<VkCommandPool>(command_pool, *m_device, vkDestroyCommandPool);
        }

        CommandPool::operator const VkCommandPool&() const {
            return m_command_pool;
        }

        std::vector<VkCommandBuffer> CommandPool::allocateCommandBuffers(uint32_t count) const {
            VkCommandBufferAllocateInfo allocate_info = {};
            allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocate_info.commandPool        = m_command_pool;
            allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocate_info.commandBufferCount = count;

            std::vector<VkCommandBuffer> command_buffers{count};
            if (vkAllocateCommandBuffers(*m_device, &allocate_info, command_buffers.data()) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate command buffers.");
            }
            return command_buffers;
        }

    }
}