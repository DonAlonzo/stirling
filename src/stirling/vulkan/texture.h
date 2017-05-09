#pragma once

#include "vulkan/vulkan.h"

#include "deleter.h"
#include "device.h"
#include "image.h"

namespace stirling {
    namespace vulkan {
        class Texture {
        public:
            Image                image;
            Deleter<VkImageView> image_view;
            Deleter<VkSampler>   sampler;

            Texture(const Device& device, Image&& image);

        private:
            Deleter<VkImageView> initImageView(const Device& device) const;
            Deleter<VkSampler>   initSampler(const Device& device) const;
        };
    }
}