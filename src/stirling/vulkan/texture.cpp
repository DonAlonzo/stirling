#include "texture.h"
#include "device.h"

namespace stirling {
    namespace vulkan {

        Texture::Texture(VkDevice device, VkImage image) :
            image_view (initImageView(device, image)),
            sampler    (initSampler(device)) {
        }

        Deleter<VkImageView> Texture::initImageView(VkDevice device, VkImage image) const {
            VkImageViewCreateInfo create_info = {};
            create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image                           = image;
            create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format                          = VK_FORMAT_R8G8B8A8_UNORM;
            create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel   = 0;
            create_info.subresourceRange.levelCount     = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount     = 1;

            VkImageView image_view;
            if (vkCreateImageView(device, &create_info, nullptr, &image_view) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create image view.");
            }
            return Deleter<VkImageView>(image_view, device, vkDestroyImageView);
        }

        Deleter<VkSampler> Texture::initSampler(VkDevice device) const {
            VkSamplerCreateInfo create_info = {};
            create_info.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            create_info.magFilter               = VK_FILTER_LINEAR;
            create_info.minFilter               = VK_FILTER_LINEAR;
            create_info.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            create_info.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            create_info.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            create_info.anisotropyEnable        = VK_TRUE;
            create_info.maxAnisotropy           = 16;
            create_info.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            create_info.unnormalizedCoordinates = VK_FALSE;
            create_info.compareEnable           = VK_FALSE;
            create_info.compareOp               = VK_COMPARE_OP_ALWAYS;
            create_info.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            create_info.mipLodBias              = 0.0f;
            create_info.minLod                  = 0.0f;
            create_info.maxLod                  = 0.0f;

            VkSampler sampler;
            if (vkCreateSampler(device, &create_info, nullptr, &sampler) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create texture sampler.");
            }
            return Deleter<VkSampler>(sampler, device, vkDestroySampler);
        }

    }
}