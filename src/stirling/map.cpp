#include "map.h"

#include "vulkan/device.h"
#include "vulkan/initializers.h"
#include "vulkan/model.h"
#include "vulkan/physical_device.h"

#include "util/memory.h"

#include "entity.h"
#include "physics_component.h"

#include <array>
#include <iostream>
#include <map>
#include <string>

namespace stirling {

    Material* MapBlueprint::createMaterial() {
        materials.emplace_back(Material());
        return &materials.back();
    }

    void MapBlueprint::addEntity(EntityCreateInfo create_info) {
        create_info_list.push_back(create_info);
    }

    void Material::addShader(VkShaderStageFlagBits stage, const char* file, const char* entry_point) {
        ShaderInfo shader_info = {};
        shader_info.stage       = stage;
        shader_info.file        = file;
        shader_info.entry_point = entry_point;
        shaders.emplace_back(shader_info);
    }

    Map MapBlueprint::instantiate(const vulkan::Device& device, VkRenderPass render_pass, VkExtent2D extent) const {
        // Map instance
        Map map = {};

        // Static uniform buffer
        map.static_uniform_buffer = device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sizeof(StaticUniformBufferObject));
        map.static_uniform_buffer.map();

        // Dynamic uniform buffer
        uint32_t number_of_dynamic_buffer_objects = create_info_list.size();
        size_t ubo_alignment = device.getPhysicalDevice().properties.limits.minUniformBufferOffsetAlignment;
        size_t dynamic_alignment = (sizeof(glm::mat4) / ubo_alignment) * ubo_alignment + ((sizeof(glm::mat4) % ubo_alignment) > 0 ? ubo_alignment : 0);
        size_t buffer_size = number_of_dynamic_buffer_objects * dynamic_alignment;

        map.dynamic_uniform_buffer_object.model = (glm::mat4*)util::memory::alignedAlloc(buffer_size, dynamic_alignment);
        map.dynamic_uniform_buffer = device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, buffer_size);
        map.dynamic_uniform_buffer.map();

        uint64_t next_dynamic_index = 0;

        // Preload textures
        std::map<std::string, vulkan::Texture&> textures;
        for (auto create_info : create_info_list) {
            if (textures.find(create_info.texture_file) == textures.end()) {
                std::cout << "Loading texture " << create_info.texture_file << std::endl;
				map.textures.emplace_back(vulkan::Texture(device, vulkan::Image::loadFromFile(device, create_info.texture_file)));
                textures.emplace(create_info.texture_file, map.textures.back());
            }
        }

        // Preload models
        std::map<std::string, vulkan::Model> models;
        for (auto create_info : create_info_list) {
            if (models.find(create_info.model_file) == models.end()) {
                std::cout << "Loading model " << create_info.model_file << std::endl;
                models.emplace(create_info.model_file, vulkan::Model::loadFromFile(create_info.model_file));
            }
        }
        std::map<std::string, vulkan::Buffer&> vertex_buffers;
        std::map<std::string, vulkan::Buffer&> index_buffers;
		std::map<std::string, size_t> index_counts;
        for (auto& model : models) {
            if (vertex_buffers.find(model.first) == vertex_buffers.end()) {
                std::cout << "Loading vertex buffer for " << model.first << std::endl;

				map.vertex_buffers.emplace_back(vulkan::Buffer(
					&device,
					VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					sizeof(vulkan::Vertex) * model.second.vertices.size()
				));

				auto& buffer = map.vertex_buffers.back();

				buffer.map();
				buffer.memcpy(model.second.vertices.data());
				buffer.unmap();

				vertex_buffers.emplace(model.first, buffer);
            }
            if (index_buffers.find(model.first) == index_buffers.end()) {
                std::cout << "Loading index buffer for " << model.first << std::endl;

				map.index_buffers.emplace_back(vulkan::Buffer(
					&device,
					VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					sizeof(uint32_t) * model.second.indices.size()
				));

				auto& buffer = map.index_buffers.back();

				buffer.map();
				buffer.memcpy(model.second.indices.data());
				buffer.unmap();

				index_buffers.emplace(model.first, buffer);
				index_counts.emplace(model.first, model.second.indices.size());
            }
        }

        // Preload shaders
        std::map<std::string, vulkan::ShaderModule> shader_modules;
        for (auto& material : materials) {
            for (auto shader : material.shaders) {
                if (shader_modules.find(shader.file) == shader_modules.end()) {
                    std::cout << "Loading shader " << shader.file << std::endl;
                    shader_modules.try_emplace(shader.file, vulkan::ShaderModule(device, shader.file));
                }
            }
        }

        // Descriptor set layout
        auto descriptor_set_layout = initDescriptorSetLayout(device);

        // Descriptor pool
        map.descriptor_pools.emplace_back(initDescriptorPool(device, create_info_list.size()));
        auto& descriptor_pool = map.descriptor_pools.back();

        // Components
		map.components.emplace_back(new PhysicsComponent{});
        auto physics_component = map.components.back().get();

        // Iterate through all entities to be created.
        for (auto create_info : create_info_list) {
            // Shaders
            std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
            for (auto shader : create_info.material->shaders) {
                shader_stages.emplace_back(vulkan::initializers::pipelineShaderStageCreateInfo(shader.stage, shader_modules.at(shader.file), shader.entry_point));
            }

            // Pipeline
            std::cout << "Creating pipeline" << std::endl;
            map.pipelines.emplace_back(vulkan::Pipeline(device, { descriptor_set_layout }, render_pass, extent, shader_stages));
            auto& pipeline = map.pipelines.back();

            // Descriptor set
            std::cout << "Allocating descriptor set" << std::endl;
            auto descriptor_set = descriptor_pool.allocateDescriptorSet(descriptor_set_layout);
        
            VkDescriptorImageInfo image_info = {};
            image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_info.imageView   = textures.at(create_info.texture_file).image_view;
            image_info.sampler     = textures.at(create_info.texture_file).sampler;

            std::vector<VkWriteDescriptorSet> write_descriptor_sets = {
                vulkan::initializers::writeDescriptorSet(descriptor_set, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &map.static_uniform_buffer.m_descriptor),
                vulkan::initializers::writeDescriptorSet(descriptor_set, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &map.dynamic_uniform_buffer.m_descriptor),
                vulkan::initializers::writeDescriptorSet(descriptor_set, 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &image_info)
            };
            vkUpdateDescriptorSets(device, write_descriptor_sets.size(), write_descriptor_sets.data(), 0, nullptr);

            // Transform (Dynamic uniform buffer)
            uint32_t dynamic_offset = next_dynamic_index++ * static_cast<uint32_t>(dynamic_alignment);
            auto transform = (glm::mat4*)(((uint64_t)map.dynamic_uniform_buffer_object.model + dynamic_offset));

            // Render instruction
			RenderInstruction render_instruction = {};
            render_instruction.pipeline_layout       = pipeline.getLayout();
            render_instruction.pipeline              = pipeline;
            render_instruction.vertex_buffers        = { vertex_buffers.at(create_info.model_file) };
            render_instruction.vertex_buffer_offsets = { 0 };
            render_instruction.index_buffer          = index_buffers.at(create_info.model_file);
			render_instruction.index_count           = index_counts.at(create_info.model_file);
            render_instruction.descriptor_sets       = { descriptor_set };
            render_instruction.dynamic_offsets       = { dynamic_offset };
            map.render_instructions.emplace_back(render_instruction);

            // Compose entity
            map.entities.emplace_back(transform);
            auto& entity = map.entities.back();

            entity.transform().moveTo(create_info.position);
            entity.transform().setRotation(create_info.rotation);
            entity.transform().setScale(create_info.scale);

            entity.addComponent(physics_component);
        }

        return map;
    }

    VkDescriptorSetLayout MapBlueprint::initDescriptorSetLayout(VkDevice device) const {
        std::vector<VkDescriptorSetLayoutBinding> bindings = {
            vulkan::initializers::descriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT),
            vulkan::initializers::descriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT),
            vulkan::initializers::descriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        VkDescriptorSetLayoutCreateInfo create_info = {};
        create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        create_info.bindingCount = bindings.size();
        create_info.pBindings    = bindings.data();

        VkDescriptorSetLayout descriptor_set_layout;
        if (vkCreateDescriptorSetLayout(device, &create_info, nullptr, &descriptor_set_layout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set layout.");
        }
        return descriptor_set_layout;
    }

    vulkan::DescriptorPool MapBlueprint::initDescriptorPool(VkDevice device, uint32_t max_sets) const {
        std::vector<VkDescriptorPoolSize> pool_sizes{3};

        pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pool_sizes[0].descriptorCount = 1;

        pool_sizes[1].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        pool_sizes[1].descriptorCount = 1;

        pool_sizes[2].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[2].descriptorCount = 1;

        return vulkan::DescriptorPool(device, pool_sizes, max_sets);
    }

}