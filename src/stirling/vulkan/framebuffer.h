#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.hpp"

#include <vector>

namespace stirling {
    namespace vulkan {
        class Framebuffer {
        public:
            Framebuffer(const Device& device, VkFramebuffer);

            operator VkFramebuffer() const;

        private:
            Deleter<VkFramebuffer> m_framebuffer;
        };
    }
}