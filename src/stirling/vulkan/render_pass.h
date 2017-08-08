#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    struct Device;
}}
#include "deleter.h"

namespace stirling {
    namespace vulkan {
        struct RenderPass {
            RenderPass(const Device& device, const VkFormat& image_format, const VkFormat& depth_format);

            operator VkRenderPass() const;

        private:
            Deleter<VkRenderPass> m_render_pass;

            Deleter<VkRenderPass> initRenderPass(const Device& device, const VkFormat& image_format, const VkFormat& depth_format) const;
        };
    }
}