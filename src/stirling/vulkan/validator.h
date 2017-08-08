#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "deleter.h"

namespace stirling {
    namespace vulkan {

        struct Validator {
            Validator(const VkInstance& instance);

            static Validator nullValidator();

        private:
            Deleter<VkDebugReportCallbackEXT> m_debug_callback;

            Validator();

            Deleter<VkDebugReportCallbackEXT> initDebugCallback(const VkInstance& instance) const;
        };

    }
}