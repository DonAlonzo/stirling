#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

namespace stirling {
	namespace vulkan {
		class RenderPass {
		public:
			RenderPass(const Device& device, const VkFormat& image_format, const VkFormat& depth_format);
			~RenderPass();
			RenderPass(RenderPass&&);
			RenderPass(const RenderPass&) = delete;
			RenderPass& operator=(RenderPass&&);
			RenderPass& operator=(const RenderPass&) = delete;

			operator VkRenderPass() const;

		private:
			const Device* m_device;
			VkRenderPass  m_render_pass;

			VkRenderPass  initRenderPass(const VkFormat& image_format, const VkFormat& depth_format) const;
		};
	}
}