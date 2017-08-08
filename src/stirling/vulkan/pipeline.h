#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        struct Device;
        struct RenderPass;
    }
}
#include "deleter.h"
#include "shader_module.h"

#include <string>

namespace stirling {
    namespace vulkan {

        struct Pipeline : Deleter<VkPipeline> {
			Deleter<VkPipelineLayout> layout;

            Pipeline(VkDevice device, std::vector<VkDescriptorSetLayout> descriptor_set_layouts, VkRenderPass render_pass, VkExtent2D extent, std::vector<VkPipelineShaderStageCreateInfo> shader_stages);

        private:
            Pipeline(VkPipelineLayout pipeline_layout, VkDevice device, VkRenderPass render_pass, VkExtent2D extent, std::vector<VkPipelineShaderStageCreateInfo> shader_stages);

            VkPipelineLayout initPipelineLayout(VkDevice device, std::vector<VkDescriptorSetLayout> descriptor_set_layouts) const;
            VkPipeline       init(VkPipelineLayout pipeline_layout, VkDevice device, VkRenderPass render_pass, VkExtent2D extent, std::vector<VkPipelineShaderStageCreateInfo> shader_stages) const;
        };

    }
}