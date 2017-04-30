#include "image_view.h"

#include "device.h"

namespace stirling {
	namespace vulkan {

		ImageView::ImageView(const Device& device, VkImageView image_view) :
			m_image_view (Deleter<VkImageView>(image_view, device, vkDestroyImageView)) {
		}

		ImageView::ImageView(const Device& device, const VkImageViewCreateInfo& create_info) :
			m_image_view (createImageView(device, create_info)) {
		}

		Deleter<VkImageView> ImageView::createImageView(const Device& device, const VkImageViewCreateInfo& create_info) const {
			VkImageView image_view;
			if (vkCreateImageView(device, &create_info, nullptr, &image_view) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create image view.");
			}
			return Deleter<VkImageView>(image_view, device, vkDestroyImageView);
		}

		ImageView::operator VkImageView() const {
			return m_image_view;
		}

	}
}