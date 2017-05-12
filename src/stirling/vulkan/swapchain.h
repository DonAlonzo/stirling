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
            Swapchain(const Device& device, VkSurfaceKHR surface, const VkExtent2D& actual_extent);
            ~Swapchain();
            Swapchain(Swapchain&&);
            Swapchain(const Swapchain&) = delete;
            Swapchain& operator=(Swapchain&&);
            Swapchain& operator=(const Swapchain&) = delete;

            void reset(const VkExtent2D& actual_extent);

            std::vector<VkFramebuffer> createFramebuffers(const RenderPass& render_pass, VkImageView depth_image_view) const;

            operator VkSwapchainKHR() const;
            const VkExtent2D& getExtent() const;
            const VkFormat&   getImageFormat() const;
            uint32_t          getImageCount() const;

        private:
            const Device*                     m_device;
            VkSurfaceKHR                      m_surface;

            SwapchainSupportDetails           m_support_details;
            VkExtent2D                        m_swapchain_extent;
            VkSurfaceFormatKHR                m_surface_format;
            VkSwapchainKHR                    m_swapchain;
            std::vector<VkImage>              m_swapchain_images;
            VkFormat                          m_swapchain_image_format;
            std::vector<Deleter<VkImageView>> m_swapchain_image_views;

            VkSwapchainKHR                    initSwapchain(VkSwapchainKHR old_swapchain);
            SwapchainSupportDetails           fetchSupportDetails(const PhysicalDevice& physical_device, VkSurfaceKHR surface) const;
            VkSurfaceFormatKHR                chooseSwapSurfaceFormat() const;
            VkPresentModeKHR                  chooseSwapPresentMode(const std::vector<VkPresentModeKHR> available_present_modes) const;
            VkExtent2D                        chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D actual_extent) const;
            std::vector<Deleter<VkImageView>> initImageViews(uint32_t image_count) const;
        };

    }
}