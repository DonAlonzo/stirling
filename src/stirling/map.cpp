#include "map.h"

#include "vulkan/device.h"
#include "vulkan/initializers.h"
#include "vulkan/model.h"
#include "vulkan/physical_device.h"

#include "util/memory.h"

#include "entity.h"
#include "model_component.h"
#include "physics_component.h"

#include <array>
#include <iostream>

namespace stirling {

    Map::Map(std::vector<EntityCreateInfo> create_info_list) :
        create_info_list (create_info_list) {
    }

    MapInstance Map::instantiate(const vulkan::Device& device, VkRenderPass render_pass, VkExtent2D extent) const {
        // Map instance
        MapInstance map_instance = {};

        // Static uniform buffer
        map_instance.static_uniform_buffer = device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sizeof(StaticUniformBufferObject));
        map_instance.static_uniform_buffer.map();

        // Dynamic uniform buffer
        uint32_t number_of_dynamic_buffer_objects = 4;
        size_t ubo_alignment = device.getPhysicalDevice().properties.limits.minUniformBufferOffsetAlignment;
        size_t dynamic_alignment = (sizeof(glm::mat4) / ubo_alignment) * ubo_alignment + ((sizeof(glm::mat4) % ubo_alignment) > 0 ? ubo_alignment : 0);
        size_t buffer_size = number_of_dynamic_buffer_objects * dynamic_alignment;

        map_instance.dynamic_uniform_buffer_object.model = (glm::mat4*)util::memory::alignedAlloc(buffer_size, dynamic_alignment);
        map_instance.dynamic_uniform_buffer = device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, buffer_size);
        map_instance.dynamic_uniform_buffer.map();

        uint64_t next_dynamic_index = 0;

        // Entity create info
        auto create_info = create_info_list.at(0);

        // Descriptor set layout
        auto descriptor_set_layout = initDescriptorSetLayout(device);

        // Descriptor pool
        map_instance.descriptor_pools.emplace_back(initDescriptorPool(device));
        auto& descriptor_pool = map_instance.descriptor_pools.back();

        // Shaders
        vulkan::ShaderModule vertex_shader(device, create_info.vertex_shader_file);
        vulkan::ShaderModule fragment_shader(device, create_info.fragment_shader_file);
        std::vector<VkPipelineShaderStageCreateInfo> shader_stages = {
            vulkan::initializers::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertex_shader, "main"),
            vulkan::initializers::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragment_shader, "main"),
        };

        // Pipeline
        map_instance.pipelines.emplace_back(vulkan::Pipeline(device, { descriptor_set_layout }, render_pass, extent, shader_stages));
        auto& pipeline = map_instance.pipelines.back();

        // Model
        auto model = vulkan::Model::loadFromFile(device, create_info.model_file, create_info.texture_file);
        auto& vertex_buffer = model.vertex_buffer;
        auto& index_buffer = model.index_buffer;
        auto& texture = model.texture;

        // Descriptor set
        auto descriptor_set = descriptor_pool.allocateDescriptorSet(descriptor_set_layout);
        
        VkDescriptorImageInfo image_info = {};
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_info.imageView   = texture.image_view;
        image_info.sampler     = texture.sampler;

        std::array<VkWriteDescriptorSet, 3> write_descriptor_sets = {
            vulkan::initializers::writeDescriptorSet(descriptor_set, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &map_instance.static_uniform_buffer.m_descriptor),
            vulkan::initializers::writeDescriptorSet(descriptor_set, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &map_instance.dynamic_uniform_buffer.m_descriptor),
            vulkan::initializers::writeDescriptorSet(descriptor_set, 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &image_info)
        };
        vkUpdateDescriptorSets(device, write_descriptor_sets.size(), write_descriptor_sets.data(), 0, nullptr);

        // Transform (Dynamic uniform buffer)
        uint32_t dynamic_offset = next_dynamic_index++ * static_cast<uint32_t>(dynamic_alignment);
        auto transform = (glm::mat4*)(((uint64_t)map_instance.dynamic_uniform_buffer_object.model + dynamic_offset));

        // Render instruction
        RenderInstruction render_instruction;
        render_instruction.pipeline_layout       = pipeline.getLayout();
        render_instruction.pipeline              = pipeline;
        render_instruction.vertex_buffers        = { vertex_buffer };
        render_instruction.vertex_buffer_offsets = { 0 };
        render_instruction.index_buffer          = index_buffer;
        render_instruction.index_count           = index_buffer.size();
        render_instruction.descriptor_sets       = { descriptor_set };
        render_instruction.dynamic_offsets       = { dynamic_offset };
        map_instance.render_instructions.emplace_back(render_instruction);

        // Components
        auto physics_component = new PhysicsComponent();
        auto model_component = new ModelComponent(std::move(model), descriptor_set);

        // Entity
        map_instance.entities.emplace_back(Entity(vulkan::Transform(transform)));
        auto& entity = map_instance.entities.back();

        entity.transform().moveTo(create_info.position);
        entity.transform().setRotation(create_info.rotation);
        entity.transform().setScale(create_info.scale);

        entity.addComponent(physics_component);
        entity.addComponent(model_component);

        return map_instance;
    }

    VkDescriptorSetLayout Map::initDescriptorSetLayout(VkDevice device) const {
        std::array<VkDescriptorSetLayoutBinding, 3> bindings = {
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

    vulkan::DescriptorPool Map::initDescriptorPool(VkDevice device) const {
        std::vector<VkDescriptorPoolSize> pool_sizes{3};

        pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pool_sizes[0].descriptorCount = 1;

        pool_sizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        pool_sizes[1].descriptorCount = 1;

        pool_sizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[2].descriptorCount = 1;

        return vulkan::DescriptorPool(device, pool_sizes, 1);
    }

}