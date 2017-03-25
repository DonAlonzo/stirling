#include "swapchain.h"

#include "device.h"
#include "physical_device.h"
#include "render_pass.h"
#include "surface.h"

#include <algorithm>

namespace stirling {
	namespace vulkan {

		Swapchain::Swapchain(const Device& device, const Surface& surface) :
			m_device           (device),
			m_support_details  (fetchSupportDetails(device.getPhysicalDevice(), surface)),
			m_swapchain_extent (chooseSwapExtent(m_support_details.capabilities)) {

			auto surface_format = chooseSwapSurfaceFormat(device.getPhysicalDevice().getSurfaceFormats(surface));
			auto present_mode = chooseSwapPresentMode(device.getPhysicalDevice().getSurfacePresentModes(surface));

			uint32_t image_count = m_support_details.capabilities.minImageCount + 1;
			if (m_support_details.capabilities.maxImageCount > 0 && image_count > m_support_details.capabilities.maxImageCount) {
				image_count = m_support_details.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR create_info = {};
			create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			create_info.surface          = surface;
			create_info.minImageCount    = image_count;
			create_info.imageFormat      = surface_format.format;
			create_info.imageColorSpace  = surface_format.colorSpace;
			create_info.imageExtent      = m_swapchain_extent;
			create_info.imageArrayLayers = 1;
			create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			auto indices = device.getPhysicalDevice().findQueueFamilies(surface);
			if (indices.graphics_family_index != indices.present_family_index) {
				uint32_t queue_family_indices[] = {
					(uint32_t)indices.graphics_family_index,
					(uint32_t)indices.present_family_index
				};

				create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
				create_info.queueFamilyIndexCount = 2;
				create_info.pQueueFamilyIndices   = queue_family_indices;
			} else {
				create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
			}

			create_info.preTransform   = m_support_details.capabilities.currentTransform;
			create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			create_info.presentMode    = present_mode;
			create_info.clipped        = VK_TRUE;
			create_info.oldSwapchain   = VK_NULL_HANDLE;

			if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, m_swapchain.replace()) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create swap chain.");
			}
			m_swapchain_images = device.getSwapchainImages(m_swapchain, image_count);
			m_swapchain_image_format = surface_format.format;
			m_swapchain_image_views = initImageViews(image_count);
		}

		SwapchainSupportDetails Swapchain::fetchSupportDetails(const PhysicalDevice& physical_device, const Surface& surface) const {
			SwapchainSupportDetails support_details = {};
			support_details.capabilities  = physical_device.getSurfaceCapabilities(surface);
			support_details.formats       = physical_device.getSurfaceFormats(surface);
			support_details.present_modes = physical_device.getSurfacePresentModes(surface);
			return support_details;
		}
	
		VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) const {
			if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED) {
				return {
					VK_FORMAT_B8G8R8A8_UNORM,
					VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
				};
			}

			for (const auto& available_format : available_formats) {
				if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return available_format;
				}
			}

			return available_formats[0];
		}
	
		VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> available_present_modes) const {
			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& available_present_mode : available_present_modes) {
				if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return available_present_mode;
				} else if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
					bestMode = available_present_mode;
				}
			}

			return bestMode;
		}

		VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const {
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return capabilities.currentExtent;
			} else {
				VkExtent2D actual_extent = { 800, 600 };

				actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
				actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));

				return actual_extent;
			}
		}

		std::vector<ImageView> Swapchain::initImageViews(uint32_t image_count) const {
			std::vector<ImageView> image_views;
			image_views.reserve(m_swapchain_images.size());

			for (uint32_t i = 0; i < m_swapchain_images.size(); ++i) {
				VkImageViewCreateInfo create_info = {};
				create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				create_info.image                           = m_swapchain_images[i];
				create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
				create_info.format                          = m_swapchain_image_format;
				create_info.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
				create_info.subresourceRange.baseMipLevel   = 0;
				create_info.subresourceRange.levelCount     = 1;
				create_info.subresourceRange.baseArrayLayer = 0;
				create_info.subresourceRange.layerCount     = 1;

				VkImageView image_view;
				if (vkCreateImageView(m_device, &create_info, nullptr, &image_view) != VK_SUCCESS) {
					throw std::runtime_error("Failed to create image views.");
				}
				image_views.push_back(ImageView(m_device, image_view));
			}

			return image_views;
		}

		const VkExtent2D& Swapchain::getExtent() const {
			return m_swapchain_extent;
		}

		const VkFormat& Swapchain::getImageFormat() const {
			return m_swapchain_image_format;
		}

		std::vector<Framebuffer> Swapchain::createFramebuffers(const RenderPass& render_pass) const {
			std::vector<Framebuffer> framebuffers;
			framebuffers.reserve(m_swapchain_image_views.size());

			for (size_t i = 0; i < m_swapchain_image_views.size(); ++i) {
				VkImageView attachments[] = {
					m_swapchain_image_views[i]
				};

				VkFramebufferCreateInfo create_info = {};
				create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				create_info.renderPass      = render_pass;
				create_info.attachmentCount = 1;
				create_info.pAttachments    = attachments;
				create_info.width           = m_swapchain_extent.width;
				create_info.height          = m_swapchain_extent.height;
				create_info.layers          = 1;

				VkFramebuffer framebuffer;
				if (vkCreateFramebuffer(m_device, &create_info, nullptr, &framebuffer) != VK_SUCCESS) {
					throw std::runtime_error("Failed to create framebuffer.");
				}
				framebuffers.push_back(Framebuffer(m_device, framebuffer));
			}
			return framebuffers;
		}

	}
}