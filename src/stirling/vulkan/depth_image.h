#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}
#include "image.h"
#include "image_view.h"

#include <vector>

namespace stirling {
	namespace vulkan {
		class DepthImage {
		public:
			DepthImage(const Device& device, const VkExtent2D& extent);

			const VkFormat&  getImageFormat() const;
			const Image&     getImage() const;
			const ImageView& getImageView() const;

		private:
			VkFormat  m_image_format;
			Image     m_image;
			ImageView m_image_view;

			VkFormat  findImageFormat(const Device& device);
			Image     createImage(const Device& device, const VkExtent2D& extent);
			ImageView createImageView(const Device& device);
		};
	}
}