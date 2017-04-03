#include "depth_image.h"

#include "device.h"
#include "physical_device.h"
#include "image.h"
#include "image_view.h"

namespace stirling {
	namespace vulkan {

		DepthImage::DepthImage(const Device& device, const VkExtent2D& extent) :
			m_image_format (findImageFormat(device)),
			m_image        (createImage(device, extent)),
			m_image_view   (createImageView(device)) {
		}

		VkFormat DepthImage::findImageFormat(const Device& device) {
			std::vector<VkFormat> candidates = {
				VK_FORMAT_D32_SFLOAT,
				VK_FORMAT_D32_SFLOAT_S8_UINT,
				VK_FORMAT_D24_UNORM_S8_UINT
			};

			for (VkFormat format : candidates) {
				VkFormatProperties properties;
				vkGetPhysicalDeviceFormatProperties(device.getPhysicalDevice(), format, &properties);

				if ((properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
					return format;
				}
			}

			throw std::runtime_error("Failed to find supported format for depth image.");
		}

		Image DepthImage::createImage(const Device& device, const VkExtent2D& extent) {
			VkImageCreateInfo create_info = {};
			create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			create_info.imageType     = VK_IMAGE_TYPE_2D;
			create_info.extent.width  = extent.width;
			create_info.extent.height = extent.height;
			create_info.extent.depth  = 1;
			create_info.mipLevels     = 1;
			create_info.arrayLayers   = 1;
			create_info.format        = m_image_format;
			create_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
			create_info.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
			create_info.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			create_info.samples       = VK_SAMPLE_COUNT_1_BIT;
			create_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
			create_info.flags         = 0;

			return Image(device, create_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		}

		ImageView DepthImage::createImageView(const Device& device) {
			VkImageViewCreateInfo create_info = {};
			create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			create_info.image                           = m_image;
			create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			create_info.format                          = m_image_format;
			create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT;
			create_info.subresourceRange.baseMipLevel   = 0;
			create_info.subresourceRange.levelCount     = 1;
			create_info.subresourceRange.baseArrayLayer = 0;
			create_info.subresourceRange.layerCount     = 1;

			return ImageView(device, create_info);
		}

		const VkFormat& DepthImage::getImageFormat() const {
			return m_image_format;
		}

		const Image& DepthImage::getImage() const {
			return m_image;
		}

		const ImageView& DepthImage::getImageView() const {
			return m_image_view;
		}

	}
}