#include "helpers.h"
#include "device.h"
#include "render_pass.h"
#include "swapchain.h"

#include <algorithm>
#include <array>

namespace stirling {
    namespace vulkan {

        Swapchain::Swapchain(const vulkan::Device& device, VkSurfaceKHR surface, VkExtent2D actual_extent, VkSwapchainKHR old_swapchain) :
            device          {device},
            support_details {fetchSupportDetails(device.physical_device, surface)},
            extent          {chooseSwapExtent(support_details.capabilities, actual_extent)},
            surface_format  {chooseSwapSurfaceFormat(helpers::getSurfaceFormats(device.physical_device, surface))},
            swapchain       {initSwapchain(device, surface, old_swapchain)},
            images          {device.getSwapchainImages(swapchain, getImageCount())},
            image_format    {surface_format.format},
            image_views     {initImageViews(getImageCount())} {
        }

        Deleter<VkSwapchainKHR> Swapchain::initSwapchain(const Device& device, VkSurfaceKHR surface, VkSwapchainKHR old_swapchain) {
            VkSwapchainCreateInfoKHR create_info = {};
            create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            create_info.surface          = surface;
            create_info.minImageCount    = getImageCount();
            create_info.imageFormat      = surface_format.format;
            create_info.imageColorSpace  = surface_format.colorSpace;
            create_info.imageExtent      = extent;
            create_info.imageArrayLayers = 1;
            create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            auto indices = device.findQueueFamilies(surface);
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

            create_info.preTransform   = support_details.capabilities.currentTransform;
            create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            create_info.presentMode    = chooseSwapPresentMode(helpers::getSurfacePresentModes(device.physical_device, surface));
            create_info.clipped        = VK_TRUE;
            create_info.oldSwapchain   = old_swapchain;

            VkSwapchainKHR swapchain;
            if (vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create swap chain.");
            }
            return Deleter<VkSwapchainKHR>(swapchain, device, vkDestroySwapchainKHR);
        }

        SwapchainSupportDetails Swapchain::fetchSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface) const {
            SwapchainSupportDetails support_details = {};
            support_details.capabilities  = helpers::getSurfaceCapabilities(physical_device, surface);
            support_details.formats       = helpers::getSurfaceFormats(physical_device, surface);
            support_details.present_modes = helpers::getSurfacePresentModes(physical_device, surface);
            return support_details;
        }
    
        VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> available_formats) const {
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

        VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D actual_extent) const {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            } else {
                actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
                actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));

                return actual_extent;
            }
        }

        std::vector<Deleter<VkImageView>> Swapchain::initImageViews(uint32_t image_count) const {
            std::vector<Deleter<VkImageView>> image_views;
            image_views.reserve(images.size());

            for (uint32_t i = 0; i < images.size(); ++i) {
                VkImageViewCreateInfo create_info = {};
                create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                create_info.image                           = images[i];
                create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
                create_info.format                          = image_format;
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
                if (vkCreateImageView(device, &create_info, nullptr, &image_view) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create image views.");
                }
                image_views.push_back(Deleter<VkImageView>(image_view, device, vkDestroyImageView));
            }

            return image_views;
        }

        std::vector<VkFramebuffer> Swapchain::createFramebuffers(const RenderPass& render_pass, VkImageView depth_image_view) const {
            std::vector<VkFramebuffer> framebuffers;
            framebuffers.reserve(image_views.size());

            for (size_t i = 0; i < image_views.size(); ++i) {
                std::array<VkImageView, 2> attachments = {
                    image_views[i],
                    depth_image_view
                };

                VkFramebufferCreateInfo create_info = {};
                create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                create_info.renderPass      = render_pass;
                create_info.attachmentCount = attachments.size();
                create_info.pAttachments    = attachments.data();
                create_info.width           = extent.width;
                create_info.height          = extent.height;
                create_info.layers          = 1;

                VkFramebuffer framebuffer;
                if (vkCreateFramebuffer(device, &create_info, nullptr, &framebuffer) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to create framebuffer.");
                }
                framebuffers.push_back(framebuffer);
            }
            return framebuffers;
        }

        Swapchain::operator VkSwapchainKHR() const {
            return swapchain;
        }

        uint32_t Swapchain::getImageCount() const {
            uint32_t image_count = support_details.capabilities.minImageCount + 1;
            if (support_details.capabilities.maxImageCount > 0 && image_count > support_details.capabilities.maxImageCount) {
                image_count = support_details.capabilities.maxImageCount;
            }
            return image_count;
        }

    }
}