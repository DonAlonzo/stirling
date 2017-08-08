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
            Swapchain(const vulkan::Device& device, VkSurfaceKHR surface, VkExtent2D actual_extent, VkSwapchainKHR old_swapchain = VK_NULL_HANDLE);
            
            std::vector<VkFramebuffer> createFramebuffers(const RenderPass& render_pass, VkImageView depth_image_view) const;

            operator VkSwapchainKHR() const;
            const VkExtent2D& getExtent() const;
            const VkFormat&   getImageFormat() const;
            uint32_t          getImageCount() const;

        private:
            VkDevice                          device;
            SwapchainSupportDetails           support_details;
            VkExtent2D                        swapchain_extent;
            VkSurfaceFormatKHR                surface_format;
            Deleter<VkSwapchainKHR>           swapchain;
            std::vector<VkImage>              swapchain_images;
            VkFormat                          swapchain_image_format;
            std::vector<Deleter<VkImageView>> swapchain_image_views;

            Deleter<VkSwapchainKHR>           initSwapchain(const Device& device, VkSurfaceKHR surface, VkSwapchainKHR old_swapchain);
            SwapchainSupportDetails           fetchSupportDetails(const PhysicalDevice& physical_device, VkSurfaceKHR surface) const;
            VkSurfaceFormatKHR                chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> available_formats) const;
            VkPresentModeKHR                  chooseSwapPresentMode(const std::vector<VkPresentModeKHR> available_present_modes) const;
            VkExtent2D                        chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D actual_extent) const;
            std::vector<Deleter<VkImageView>> initImageViews(uint32_t image_count) const;
        };

    }
}