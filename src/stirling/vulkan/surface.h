#pragma once

// Vulkan
#include "vulkan/vulkan.h"

#include "deleter.h"

namespace stirling {
    namespace vulkan {

        class Surface {
        public:
            Surface(vulkan::Deleter<VkSurfaceKHR> surface);

            operator const VkSurfaceKHR&() const;

        private:
            Deleter<VkSurfaceKHR> surface;
        };

    }
}