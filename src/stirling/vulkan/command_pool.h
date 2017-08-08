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

        struct CommandPool : Deleter<VkCommandPool>{
            CommandPool(const Device& device, int queue_family_index);

            std::vector<VkCommandBuffer> allocateCommandBuffers(uint32_t count) const;

        private:
            const Device& device;

            VkCommandPool initCommandPool(VkDevice device, int queue_family_index) const;
        };

    }
}