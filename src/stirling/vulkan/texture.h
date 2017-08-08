#pragma once

#include "vulkan/vulkan.h"

#include "deleter.h"
#include "device.h"
#include "image.h"

namespace stirling {
    namespace vulkan {

        struct Texture {
            Deleter<VkImageView> image_view;
            Deleter<VkSampler>   sampler;

            Texture(VkDevice device, VkImage image);

        private:
            Deleter<VkImageView> initImageView(VkDevice device, VkImage image) const;
            Deleter<VkSampler>   initSampler(VkDevice device) const;
        };

    }
}