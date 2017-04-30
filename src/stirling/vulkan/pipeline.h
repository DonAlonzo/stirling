#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        class Device;
        class RenderPass;
    }
}
#include "deleter.hpp"
#include "shader_module.h"

namespace stirling {
    namespace vulkan {
        class Pipeline {
        public:
            Pipeline(const Device& device, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts, const RenderPass& render_pass, const VkExtent2D& extent);

            operator VkPipeline() const;

            const VkPipelineLayout& getLayout() const;

        private:
            Deleter<VkPipelineLayout> m_pipeline_layout;
            Deleter<VkPipeline>       m_pipeline;
            
            Deleter<VkPipelineLayout> initPipelineLayout(const Device& device, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts) const;
            Deleter<VkPipeline>       initPipeline(const Device& device, const RenderPass& render_pass, const VkExtent2D& extent) const;
        };
    }
}