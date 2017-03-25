#include "render_pass.h"
#include "device.h"

namespace stirling {
	namespace vulkan {

		RenderPass::RenderPass(const Device& device, const VkFormat& image_format) :
			m_device      (device),
			m_render_pass (initRenderPass(image_format)) {
		}

		void RenderPass::reset(const VkFormat& image_format) {
			vkDestroyRenderPass(m_device, m_render_pass, nullptr);
			m_render_pass = initRenderPass(image_format);
		}

		RenderPass::~RenderPass() {
			vkDestroyRenderPass(m_device, m_render_pass, nullptr);
		}

		VkRenderPass RenderPass::initRenderPass(const VkFormat& image_format) const {
			VkAttachmentDescription color_attachment = {};
			color_attachment.format         = image_format;
			color_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
			color_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
			color_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
			color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
			color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference color_attachment_reference = {};
			color_attachment_reference.attachment = 0;
			color_attachment_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments    = &color_attachment_reference;

			VkSubpassDependency dependency = {};
			dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass    = 0;
			dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo create_info = {};
			create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			create_info.attachmentCount = 1;
			create_info.pAttachments    = &color_attachment;
			create_info.subpassCount    = 1;
			create_info.pSubpasses      = &subpass;
			create_info.dependencyCount = 1;
			create_info.pDependencies   = &dependency;

			VkRenderPass render_pass;
			if (vkCreateRenderPass(m_device, &create_info, nullptr, &render_pass) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create render pass.");
			}
			return render_pass;
		}

		RenderPass::operator VkRenderPass() const {
			return m_render_pass;
		}

	}
}