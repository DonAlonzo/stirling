#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    struct Device;
}}
#include "deleter.h"

namespace stirling {
    namespace vulkan {

        struct Semaphore {
            friend struct Device;

        public:
            operator VkSemaphore() const;
            
        private:
            Semaphore(const Device& device);

            Deleter<VkSemaphore> m_semaphore;

            Deleter<VkSemaphore> initSemaphore(const Device& device) const;
        };

    }
}