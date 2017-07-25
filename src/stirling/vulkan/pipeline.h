#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        class Device;
        class RenderPass;
    }
}
#include "deleter.h"
#include "shader_module.h"

#include <string>

namespace stirling {
    namespace vulkan {
        struct Pipeline {
			Deleter<VkPipelineLayout> layout;
			Deleter<VkPipeline>       pipeline;

            Pipeline(VkDevice device, std::vector<VkDescriptorSetLayout> descriptor_set_layouts, VkRenderPass render_pass, VkExtent2D extent, std::vector<VkPipelineShaderStageCreateInfo> shader_stages);

            operator VkPipeline() const;

        private:
            Deleter<VkPipelineLayout> initPipelineLayout(VkDevice device, std::vector<VkDescriptorSetLayout> descriptor_set_layouts) const;
            Deleter<VkPipeline>       initPipeline(VkDevice device, VkRenderPass render_pass, VkExtent2D extent, std::vector<VkPipelineShaderStageCreateInfo> shader_stages) const;
        };
    }
}