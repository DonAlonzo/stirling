#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.hpp"

namespace stirling {
    namespace vulkan {
        class ImageView {
        public:
            ImageView(const Device& device, VkImageView);
            ImageView(const Device& device, const VkImageViewCreateInfo& create_info);

            operator VkImageView() const;

        private:
            Deleter<VkImageView> m_image_view;
            
            Deleter<VkImageView> createImageView(const Device& device, const VkImageViewCreateInfo& create_info) const;
        };
    }
}