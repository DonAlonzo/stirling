#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "deleter.h"

namespace stirling {
    namespace vulkan {

        struct Validator {
            Validator();
            Validator(const VkInstance& instance);

        private:
            Deleter<VkDebugReportCallbackEXT> debug_callback;

            Deleter<VkDebugReportCallbackEXT> initDebugCallback(const VkInstance& instance) const;
        };

    }
}