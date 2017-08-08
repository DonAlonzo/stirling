#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        struct Device;
    }
}
#include "deleter.h"

#include <vector>

namespace stirling {
    namespace vulkan {

        struct CommandPool {
            CommandPool(const Device& device, int queue_family_index);

            operator const VkCommandPool&() const;

            std::vector<VkCommandBuffer> allocateCommandBuffers(uint32_t count) const;

        private:
            const Device&          m_device;
            Deleter<VkCommandPool> m_command_pool;

            Deleter<VkCommandPool> initCommandPool(int queue_family_index) const;
        };

    }
}