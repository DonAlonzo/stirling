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
			RenderPass(const Device& device, const VkFormat& image_format);
			~RenderPass();
			RenderPass(RenderPass&&) = default;
			RenderPass(const RenderPass&) = delete;
			RenderPass& operator=(RenderPass&&) = delete;
			RenderPass& operator=(const RenderPass&) = delete;

			void reset(const VkFormat& image_format);

			operator VkRenderPass() const;

		private:
			const Device& m_device;
			VkRenderPass  m_render_pass;

			VkRenderPass  initRenderPass(const VkFormat& image_format) const;
		};
	}
}