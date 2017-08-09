#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
namespace stirling {
    namespace vulkan {
        struct Device;
        struct RenderPass;
        struct Surface;
    }
}
#include "deleter.h"
#include "device.h"

#include <vector>

namespace stirling {
    namespace vulkan {

        struct SwapchainSupportDetails {
            VkSurfaceCapabilitiesKHR        capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR>   present_modes;
        };

        struct Swapchain {
        private:
            VkDevice                          device;
        public:
            SwapchainSupportDetails           support_details;
            VkExtent2D                        extent;
            VkSurfaceFormatKHR                surface_format;
        private:
            Deleter<VkSwapchainKHR>           swapchain;
        public:
            std::vector<VkImage>              images;
            VkFormat                          image_format;
            std::vector<Deleter<VkImageView>> image_views;

            Swapchain(const vulkan::Device& device, VkSurfaceKHR surface, VkExtent2D actual_extent, VkSwapchainKHR old_swapchain = VK_NULL_HANDLE);
            
            operator VkSwapchainKHR() const;

            std::vector<VkFramebuffer> createFramebuffers(const RenderPass& render_pass, VkImageView depth_image_view) const;

        private:
            Deleter<VkSwapchainKHR>           initSwapchain(const Device& device, VkSurfaceKHR surface, VkSwapchainKHR old_swapchain);
            SwapchainSupportDetails           fetchSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface) const;
            VkSurfaceFormatKHR                chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> available_formats) const;
            VkPresentModeKHR                  chooseSwapPresentMode(const std::vector<VkPresentModeKHR> available_present_modes) const;
            VkExtent2D                        chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D actual_extent) const;
            std::vector<Deleter<VkImageView>> initImageViews(uint32_t image_count) const;
            uint32_t                          getImageCount() const;
        };

    }
}