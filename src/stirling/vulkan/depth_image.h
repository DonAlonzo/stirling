#pragma once

#include "vulkan/vulkan.h"

#include "deleter.h"
#include "device.h"
#include "image.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        class DepthImage {
        public:
            VkFormat             image_format;
            Image                image;
            Deleter<VkImageView> image_view;

            DepthImage(const Device& device, const VkExtent2D& extent);

        private:

            VkFormat             findImageFormat(const Device& device);
            Image                createImage(const Device& device, const VkExtent2D& extent);
            Deleter<VkImageView> createImageView(const Device& device);
        };
    }
}