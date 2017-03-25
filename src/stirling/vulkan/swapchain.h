#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
namespace stirling {
	namespace vulkan {
		class Device;
		class RenderPass;
		class Surface;
	}
}
#include "deleter.h"
#include "device.h"
#include "framebuffer.h"
#include "image_view.h"

#include <vector>

namespace stirling {
	namespace vulkan {

		struct SwapchainSupportDetails {
			VkSurfaceCapabilitiesKHR        capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR>   present_modes;
		};

		class Swapchain {
		public:
			Swapchain(const Device& device, const Surface& surface);
			~Swapchain() = default;
			Swapchain(Swapchain&&) = default;
			Swapchain(const Swapchain&) = delete;
			Swapchain& operator=(Swapchain&&) = delete;
			Swapchain& operator=(const Swapchain&) = delete;

			const VkExtent2D& getExtent() const;
			const VkFormat&   getImageFormat() const;

			std::vector<Framebuffer> createFramebuffers(const RenderPass& render_pass) const;

		private:
			const Device&            m_device;

			SwapchainSupportDetails  m_support_details;
			Deleter<VkSwapchainKHR>  m_swapchain{m_device, vkDestroySwapchainKHR};
			VkExtent2D               m_swapchain_extent;
			std::vector<VkImage>     m_swapchain_images;
			VkFormat                 m_swapchain_image_format;
			std::vector<ImageView>   m_swapchain_image_views;

			SwapchainSupportDetails  fetchSupportDetails(const PhysicalDevice& physical_device, const Surface& surface) const;
			VkSurfaceFormatKHR       chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
			VkPresentModeKHR         chooseSwapPresentMode(const std::vector<VkPresentModeKHR> available_present_modes) const;
			VkExtent2D               chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
			std::vector<ImageView>   initImageViews(uint32_t image_count) const;
		};

	}
}