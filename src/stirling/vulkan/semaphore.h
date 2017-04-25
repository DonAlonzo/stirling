#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        class Device;
    }
}

namespace stirling {
    namespace vulkan {
        class Semaphore {
            friend class Device;

        public:
            ~Semaphore();
            Semaphore(Semaphore&&);
            Semaphore(const Semaphore&) = delete;
            Semaphore& operator=(Semaphore&&);
            Semaphore& operator=(const Semaphore&) = delete;

            operator VkSemaphore() const;
            
        private:
            Semaphore(const Device& device);

            const Device* m_device;
            VkSemaphore   m_semaphore;

            VkSemaphore   initSemaphore() const;
        };
    }
}