#include "shader_module.h"

#include "pipeline.h"
#include "device.h"

namespace stirling {
    namespace vulkan {

        ShaderModule::ShaderModule(VkDevice device, const std::string& file_name) :
            Deleter<VkShaderModule>(createShaderModule(device, util::io::readFile(file_name)), device, vkDestroyShaderModule) {
        }

        VkShaderModule ShaderModule::createShaderModule(VkDevice device, const std::vector<char>& code) const {
            VkShaderModuleCreateInfo create_info = {};
            create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            create_info.codeSize = code.size();
            create_info.pCode    = (uint32_t*)code.data();

            VkShaderModule shader_module;
            if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create shader module.");
            }
            return shader_module;
        }

    }
}