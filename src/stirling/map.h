#pragma once

namespace stirling { namespace vulkan { class Device; } }
#include "vulkan/descriptor_pool.h"
#include "vulkan/index_buffer.h"
#include "vulkan/pipeline.h"
#include "vulkan/shader_module.h"
#include "vulkan/texture.h"
#include "vulkan/vertex_buffer.h"

#include "entity.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <vector>

namespace stirling {

    struct StaticUniformBufferObject {
        glm::mat4 projection;
        glm::mat4 view;
    };

    struct DynamicUniformBufferObject {
        glm::mat4* model = nullptr;
    };
    
    struct ComponentCreateInfo {
    };

    struct EntityCreateInfo {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        const char* model_file;
        const char* texture_file;
        const char* vertex_shader_file;
        const char* fragment_shader_file;
    };

    struct RenderInstruction {
        VkPipelineLayout             pipeline_layout;
        VkPipeline                   pipeline;
        std::vector<VkDeviceSize>    vertex_buffer_offsets;
        std::vector<VkBuffer>        vertex_buffers;
        VkBuffer                     index_buffer;
        uint32_t                     index_count;
        std::vector<VkDescriptorSet> descriptor_sets;
        std::vector<uint32_t>        dynamic_offsets;
    };

    struct MapInstance {
        StaticUniformBufferObject               static_uniform_buffer_object;
        DynamicUniformBufferObject              dynamic_uniform_buffer_object;
        vulkan::Buffer                          static_uniform_buffer;
        vulkan::Buffer                          dynamic_uniform_buffer;
        std::vector<vulkan::ShaderModule>       shaders;
        std::vector<vulkan::VertexBuffer>       vertex_buffers;
        std::vector<vulkan::IndexBuffer>        index_buffers;
        std::vector<vulkan::Texture>            textures;
        std::vector<vulkan::DescriptorPool>     descriptor_pools;
        std::vector<vulkan::Pipeline>           pipelines;
        std::vector<RenderInstruction>          render_instructions;
        std::vector<std::unique_ptr<Component>> components;
        std::vector<Entity>                     entities;
    };

    class Map {
    public:
        std::vector<EntityCreateInfo> create_info_list;

        Map(std::vector<EntityCreateInfo> create_info_list);

        MapInstance instantiate(const vulkan::Device& device, VkRenderPass render_pass, VkExtent2D extent) const;

    private:
        VkDescriptorSetLayout initDescriptorSetLayout(VkDevice device) const;
        vulkan::DescriptorPool initDescriptorPool(VkDevice device, uint32_t max_sets) const;
    };

}