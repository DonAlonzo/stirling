#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}
#include "shader_module.h"

namespace stirling {
	namespace vulkan {
		class Pipeline {
		public:
			Pipeline(const Device& device, const VkExtent2D& extent);
			~Pipeline();
			Pipeline(Pipeline&&) = default;
			Pipeline(const Pipeline&) = delete;
			Pipeline& operator=(Pipeline&&) = default;
			Pipeline& operator=(const Pipeline&) = delete;

			const Device& getDevice() const;

		private:
			const Device&    m_device;

			VkPipelineLayout m_pipeline_layout;
			ShaderModule     m_vertex_shader;
			ShaderModule     m_fragment_shader;

			VkPipelineLayout initPipelineLayout(const Device& device, const VkExtent2D& extent);
		};
	}
}