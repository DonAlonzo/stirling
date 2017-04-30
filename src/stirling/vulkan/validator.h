#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "deleter.hpp"

namespace stirling {
    namespace vulkan {

        class Validator {
        public:
            Validator(const VkInstance& instance);

            static Validator nullValidator();

        private:
            Deleter<VkDebugReportCallbackEXT> m_debug_callback;

            Validator();

            Deleter<VkDebugReportCallbackEXT> initDebugCallback(const VkInstance& instance) const;
        };

    }
}