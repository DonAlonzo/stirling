#include "shader_module.h"
#include "pipeline.h"
#include "device.h"

#include <fstream>

namespace stirling {
	namespace vulkan {
		ShaderModule::ShaderModule(const Pipeline& pipeline, const std::string& file_name) :
			m_pipeline      (&pipeline),
			m_shader_module (createShaderModule(readFile(file_name))) {
		}

		VkShaderModule ShaderModule::createShaderModule(const std::vector<char>& code) {
			VkShaderModuleCreateInfo create_info = {};
			create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			create_info.codeSize = code.size();
			create_info.pCode    = (uint32_t*) code.data();

			VkShaderModule shader_module;
			if (vkCreateShaderModule(m_pipeline->getDevice(), &create_info, nullptr, &shader_module) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create shader module.");
			}
			return shader_module;
		}

        ShaderModule::ShaderModule(ShaderModule&& rhs) :
            m_pipeline      (std::move(rhs.m_pipeline)),
            m_shader_module (std::move(rhs.m_shader_module)) {

            rhs.m_shader_module = VK_NULL_HANDLE;
        }
        
        ShaderModule& ShaderModule::operator=(ShaderModule&& rhs) {
            if (m_shader_module != VK_NULL_HANDLE) vkDestroyShaderModule(m_pipeline->getDevice(), m_shader_module, nullptr);

            m_shader_module = std::move(rhs.m_shader_module);
            m_pipeline      = std::move(rhs.m_pipeline);

            rhs.m_shader_module = VK_NULL_HANDLE;
            
            return *this;
        }

		ShaderModule::~ShaderModule() {
            if (m_shader_module != VK_NULL_HANDLE) vkDestroyShaderModule(m_pipeline->getDevice(), m_shader_module, nullptr);
		}

		ShaderModule::operator VkShaderModule() const {
			return m_shader_module;
		}

		std::vector<char> ShaderModule::readFile(const std::string& file_name) {
			std::ifstream file(file_name, std::ios::ate | std::ios::binary);

			if (!file.is_open()) throw std::runtime_error("Failed to open file.");

			auto file_size = (size_t)file.tellg();
			std::vector<char> buffer(file_size);
			file.seekg(0);
			file.read(buffer.data(), file_size);
			file.close();
			return buffer;
		}
	}
}