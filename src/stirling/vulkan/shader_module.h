#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    struct Device;
}}
#include "deleter.h"
#include "../util/io.h"

#include <string>
#include <vector>

namespace stirling {
    namespace vulkan {

        struct ShaderModule {
            ShaderModule(VkDevice device, const std::string& file_name);

            operator const VkShaderModule&() const;

        private:
            Deleter<VkShaderModule> m_shader_module;

            Deleter<VkShaderModule> createShaderModule(VkDevice device, const std::vector<char>& code) const;
        };

    }
}