#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
namespace stirling {
	namespace vulkan {
		class Device;
		class Surface;
	}
}
#include "device.h"
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
			~Swapchain();
			Swapchain(Swapchain&&) = default;
			Swapchain(const Swapchain&) = delete;
			Swapchain& operator=(Swapchain&&) = default;
			Swapchain& operator=(const Swapchain&) = delete;

			const VkExtent2D& getExtent() const;

		private:
			const Device&           m_device;

			SwapchainSupportDetails m_support_details;
			VkSwapchainKHR          m_swapchain;
			std::vector<VkImage>    m_swapchain_images;
			VkFormat                m_swapchain_image_format;
			VkExtent2D              m_swapchain_extent;
			std::vector<ImageView>  m_swapchain_image_views;

			SwapchainSupportDetails fetchSupportDetails(const PhysicalDevice& physical_device, const Surface& surface) const;
			VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
			VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR> available_present_modes) const;
			VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
			std::vector<ImageView>  createImageViews() const;
		};

	}
}