#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Pipeline;
	}
}

#include <string>
#include <vector>

namespace stirling {
	namespace vulkan {
		class ShaderModule {
		public:
			ShaderModule(const Pipeline& pipeline, const std::string& file_name);
			~ShaderModule();
			ShaderModule(ShaderModule&&) = default;
			ShaderModule(const ShaderModule&) = delete;
			ShaderModule& operator=(ShaderModule&&) = delete;
			ShaderModule& operator=(const ShaderModule&) = delete;

			operator VkShaderModule() const;

		private:
			const Pipeline* m_pipeline;

			VkShaderModule  m_shader_module;

			VkShaderModule  createShaderModule(const std::vector<char>& code);

			std::vector<char> readFile(const std::string& file_name);
		};
	}
}