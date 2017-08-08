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

            operator const VkQueue&() const;

            CommandPool createCommandPool() const;

        private:
            const Device& m_device;
            int           m_family_index;
            VkQueue       m_queue;

            VkQueue       initQueue() const;
        };

    }
}