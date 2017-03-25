#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

namespace stirling {
	namespace vulkan {
		class ImageView {
		public:
			ImageView(const Device& device, VkImageView);
			~ImageView();
			ImageView(ImageView&& rhs);
			ImageView(const ImageView&) = delete;
			ImageView& operator=(ImageView&&) = delete;
			ImageView& operator=(const ImageView&) = delete;

			operator VkImageView() const;

		private:
			const Device& m_device;

			VkImageView   m_image_view;
		};
	}
}