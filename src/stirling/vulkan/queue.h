#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        struct CommandPool;
        struct Device;
    }
}

namespace stirling {
    namespace vulkan {

        struct Queue {
            Queue(const Device& device, int family_index);

            operator VkQueue() const;

            CommandPool createCommandPool() const;

        private:
            const Device& device;
            int           family_index;
            VkQueue       queue;

            VkQueue       initQueue() const;
        };

    }
}