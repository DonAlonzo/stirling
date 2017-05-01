#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.h"

namespace stirling {
    namespace vulkan {
        class Semaphore {
            friend class Device;

        public:
            operator VkSemaphore() const;
            
        private:
            Semaphore(const Device& device);

            Deleter<VkSemaphore> m_semaphore;

            Deleter<VkSemaphore> initSemaphore(const Device& device) const;
        };
    }
}