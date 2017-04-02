#include "texture.h"
#include "device.h"

namespace stirling {
	namespace vulkan {

		Texture::Texture(const Device& device, Image&& image) :
			m_device     (device),
			m_image      (std::move(image)),
			m_image_view (initImageView()),
			m_sampler    (initSampler()) {
		}

		ImageView Texture::initImageView() const {
			VkImageViewCreateInfo create_info = {};
			create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			create_info.image                           = m_image;
			create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			create_info.format                          = VK_FORMAT_R8G8B8A8_UNORM;
			create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			create_info.subresourceRange.baseMipLevel   = 0;
			create_info.subresourceRange.levelCount     = 1;
			create_info.subresourceRange.baseArrayLayer = 0;
			create_info.subresourceRange.layerCount     = 1;

			return ImageView(m_device, create_info);
		}

		Sampler Texture::initSampler() const {
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

			return Sampler(m_device, create_info);
		}

		const Image& Texture::getImage() const {
			return m_image;
		}

		const ImageView& Texture::getImageView() const {
			return m_image_view;
		}

		const Sampler& Texture::getSampler() const {
			return m_sampler;
		}

	}
}