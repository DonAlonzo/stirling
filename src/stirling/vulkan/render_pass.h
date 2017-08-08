#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    struct Device;
}}
#include "deleter.h"

namespace stirling {
    namespace vulkan {

        struct RenderPass : Deleter<VkRenderPass> {
            RenderPass(const Device& device, const VkFormat& image_format, const VkFormat& depth_format);

        private:
            VkRenderPass initRenderPass(const Device& device, const VkFormat& image_format, const VkFormat& depth_format) const;
        };

    }
}