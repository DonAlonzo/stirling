#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
		class RenderPass;
	}
}
#include "shader_module.h"

namespace stirling {
	namespace vulkan {
		class Pipeline {
		public:
			Pipeline(const Device& device, const RenderPass& render_pass, const VkExtent2D& extent);
			~Pipeline();
			Pipeline(Pipeline&&) = default;
			Pipeline(const Pipeline&) = delete;
			Pipeline& operator=(Pipeline&&) = delete;
			Pipeline& operator=(const Pipeline&) = delete;

			operator VkPipeline() const;

			const Device& getDevice() const;

		private:
			const Device&    m_device;

			VkPipelineLayout m_pipeline_layout;
			VkPipeline       m_pipeline;

			VkPipelineLayout initPipelineLayout() const;
			VkPipeline       initPipeline(const RenderPass& render_pass, const VkExtent2D& extent) const;
		};
	}
}