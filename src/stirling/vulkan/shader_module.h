#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    class Device;
}}
#include "deleter.hpp"

#include <string>
#include <vector>

namespace stirling {
    namespace vulkan {
        class ShaderModule {
        public:
            ShaderModule(const Device& device, const std::string& file_name);

            operator const VkShaderModule&() const;

        private:
            Deleter<VkShaderModule> m_shader_module;

            Deleter<VkShaderModule> createShaderModule(const Device& device, const std::vector<char>& code) const;

            std::vector<char> readFile(const std::string& file_name);
        };
    }
}