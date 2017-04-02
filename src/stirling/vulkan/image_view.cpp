#include "image_view.h"

#include "device.h"

namespace stirling {
	namespace vulkan {

		ImageView::ImageView(const Device& device, VkImageView image_view) :
			m_device     (device),
			m_image_view (image_view) {
		}

		ImageView::ImageView(const Device& device, const VkImageViewCreateInfo& create_info) :
			m_device     (device),
			m_image_view (createImageView(create_info)) {
		}

		VkImageView ImageView::createImageView(const VkImageViewCreateInfo& create_info) const {
			VkImageView image_view;
			if (vkCreateImageView(m_device, &create_info, nullptr, &image_view) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create image view.");
			}
			return image_view;
		}

		ImageView::ImageView(ImageView&& rhs) :
			m_device     (std::move(rhs.m_device)),
			m_image_view (std::move(rhs.m_image_view)) {
			rhs.m_image_view = VK_NULL_HANDLE;
		}

		ImageView::~ImageView() {
			if (m_image_view != VK_NULL_HANDLE) {
				vkDestroyImageView(m_device, m_image_view, nullptr);
			}
		}

		ImageView::operator VkImageView() const {
			return m_image_view;
		}

	}
}