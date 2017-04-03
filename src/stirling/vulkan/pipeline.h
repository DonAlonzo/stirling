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
			Pipeline(Pipeline&&);
			Pipeline(const Pipeline&) = delete;
			Pipeline& operator=(Pipeline&&);
			Pipeline& operator=(const Pipeline&) = delete;

			operator VkPipeline() const;

			const Device& getDevice() const;
			const VkDescriptorSetLayout& getDescriptorSetLayout() const;
			const VkPipelineLayout& getLayout() const;

		private:
			const Device*         m_device;
			VkDescriptorSetLayout m_descriptor_set_layout;
			VkPipelineLayout      m_pipeline_layout;
			VkPipeline            m_pipeline;
			
			VkDescriptorSetLayout initDescriptorSetLayout() const;
			VkPipelineLayout      initPipelineLayout() const;
			VkPipeline            initPipeline(const RenderPass& render_pass, const VkExtent2D& extent) const;
		};
	}
}