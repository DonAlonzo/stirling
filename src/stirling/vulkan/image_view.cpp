#include "image_view.h"

#include "device.h"

namespace stirling {
	namespace vulkan {

		ImageView::ImageView(const Device& device, VkImageView image_view) :
			m_device     (device),
			m_image_view (image_view) {
		}

		ImageView::~ImageView() {
			vkDestroyImageView(m_device, m_image_view, nullptr);
		}

	}
}