#pragma once

namespace stirling { namespace vulkan { class Device; } }
#include "vulkan/buffer.h"
#include "vulkan/descriptor_pool.h"
#include "vulkan/pipeline.h"
#include "vulkan/shader_module.h"
#include "vulkan/texture.h"

#include "entity.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace stirling {

    struct StaticUniformBufferObject {
        glm::mat4 projection;
        glm::mat4 view;
    };

    struct DynamicUniformBufferObject {
        glm::mat4* model = nullptr;
    };

    struct ShaderInfo {
		std::string           file;
		std::string           entry_point;
        VkShaderStageFlagBits stage;
    };

    struct Material {
        std::vector<ShaderInfo> shaders;

        void addShader(VkShaderStageFlagBits stage, const std::string& file, const std::string& entry_point);
    };

    struct EntityCreateInfo {
        glm::vec3   position;
        glm::vec3   rotation;
        glm::vec3   scale;
        std::string model_file;
		std::string texture_file;
        Material*   material;
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

    struct Map {
        StaticUniformBufferObject               static_uniform_buffer_object;
        DynamicUniformBufferObject              dynamic_uniform_buffer_object;
        vulkan::Buffer                          static_uniform_buffer;
        vulkan::Buffer                          dynamic_uniform_buffer;
		vulkan::MemoryMapping                   static_uniform_buffer_mapping;
		vulkan::MemoryMapping                   dynamic_uniform_buffer_mapping;
        std::vector<vulkan::ShaderModule>       shaders;
        std::vector<vulkan::Buffer>             vertex_buffers;
        std::vector<vulkan::Buffer>             index_buffers;
		std::vector<vulkan::Image>              images;
        std::vector<vulkan::Texture>            textures;
        std::vector<vulkan::DescriptorPool>     descriptor_pools;
        std::vector<vulkan::Pipeline>           pipelines;
        std::vector<RenderInstruction>          render_instructions;
        std::vector<std::unique_ptr<Component>> components;
        std::vector<Entity>                     entities;

		Map(vulkan::Buffer&& static_uniform_buffer, vulkan::Buffer&& dynamic_uniform_buffer);
    };

    class MapBlueprint {
    public:
        Map instantiate(const vulkan::Device& device, VkRenderPass render_pass, VkExtent2D extent) const;

        Material* createMaterial();
        void addEntity(EntityCreateInfo);

    private:
        std::vector<Material> materials;
        std::vector<EntityCreateInfo> create_info_list;

        VkDescriptorSetLayout initDescriptorSetLayout(VkDevice device) const;
        vulkan::DescriptorPool initDescriptorPool(VkDevice device, uint32_t max_sets) const;

		std::unordered_map<std::string, vulkan::ShaderModule> preloadShaders(VkDevice device) const;
    };

}