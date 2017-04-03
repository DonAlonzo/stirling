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
			Swapchain(const Device& device, const Surface& surface, const VkExtent2D& actual_extent);
			~Swapchain();
			Swapchain(Swapchain&&);
			Swapchain(const Swapchain&) = delete;
			Swapchain& operator=(Swapchain&&);
			Swapchain& operator=(const Swapchain&) = delete;

			void reset(const VkExtent2D& actual_extent);

			std::vector<Framebuffer> createFramebuffers(const RenderPass& render_pass, const ImageView& depth_image_view) const;

			operator VkSwapchainKHR() const;
			const VkExtent2D& getExtent() const;
			const VkFormat&   getImageFormat() const;
			uint32_t          getImageCount() const;

		private:
			const Device*           m_device;
			const Surface*          m_surface;

			SwapchainSupportDetails m_support_details;
			VkExtent2D              m_swapchain_extent;
			VkSurfaceFormatKHR      m_surface_format;
			VkSwapchainKHR          m_swapchain;
			std::vector<VkImage>    m_swapchain_images;
			VkFormat                m_swapchain_image_format;
			std::vector<ImageView>  m_swapchain_image_views;

			VkSwapchainKHR          initSwapchain(VkSwapchainKHR old_swapchain);
			SwapchainSupportDetails fetchSupportDetails(const PhysicalDevice& physical_device, const Surface& surface) const;
			VkSurfaceFormatKHR      chooseSwapSurfaceFormat() const;
			VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR> available_present_modes) const;
			VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D actual_extent) const;
			std::vector<ImageView>  initImageViews(uint32_t image_count) const;
		};

	}
}