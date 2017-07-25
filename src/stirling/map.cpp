#include "map.h"

#include "vulkan/device.h"
#include "vulkan/initializers.h"
#include "vulkan/model.h"
#include "vulkan/physical_device.h"

#include "util/memory.h"

#include "entity.h"
#include "physics_component.h"

#include <array>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

namespace stirling {

	template<class T>
	inline void hash_combine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

    Material* MapBlueprint::createMaterial() {
		materials.emplace_back(Material{});
        return &materials.back();
    }

    void MapBlueprint::addEntity(EntityCreateInfo create_info) {
        create_info_list.push_back(create_info);
    }

    void Material::addShader(VkShaderStageFlagBits stage, const std::string& file, const std::string& entry_point) {
        ShaderInfo shader_info = {};
        shader_info.stage       = stage;
        shader_info.file        = file;
        shader_info.entry_point = entry_point;
        shaders.emplace_back(shader_info);
    }

    Map MapBlueprint::instantiate(const vulkan::Device& device, VkRenderPass render_pass, VkExtent2D extent) const {
		// Calculate size and alignment of dynamic buffer
        uint32_t number_of_dynamic_buffer_objects = create_info_list.size();
        size_t ubo_alignment = device.getPhysicalDevice().properties.limits.minUniformBufferOffsetAlignment;
        size_t dynamic_alignment = (sizeof(glm::mat4) / ubo_alignment) * ubo_alignment + ((sizeof(glm::mat4) % ubo_alignment) > 0 ? ubo_alignment : 0);
        size_t buffer_size = number_of_dynamic_buffer_objects * dynamic_alignment;

		// Map instance
		Map map(
			// Static uniform buffer
			device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sizeof(StaticUniformBufferObject)),
			// Dynamic uniform buffer
			device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, buffer_size)
		);

		// Dynamic uniform buffer object + index counter
		map.dynamic_uniform_buffer_object.model = (glm::mat4*)util::memory::alignedAlloc(buffer_size, dynamic_alignment);
		uint64_t next_dynamic_index = 0;

        // Preload textures
		std::unordered_map<std::string, std::pair<VkImageView, VkSampler>> textures;
        for (auto create_info : create_info_list) {
            if (textures.find(create_info.texture_file) == textures.end()) {
                std::cout << "Loading texture " << create_info.texture_file << std::endl;
				
				map.images.emplace_back(vulkan::Image::loadFromFile(device, create_info.texture_file));
				map.textures.emplace_back(vulkan::Texture(device, map.images.back()));

				textures.emplace(create_info.texture_file, std::make_pair((VkImageView)map.textures.back().image_view, (VkSampler)map.textures.back().sampler));
            }
        }

        // Preload models
        std::unordered_map<std::string, vulkan::Model> models;
        for (auto create_info : create_info_list) {
            if (models.find(create_info.model_file) == models.end()) {
                std::cout << "Loading model " << create_info.model_file << std::endl;
                models.emplace(create_info.model_file, vulkan::Model::loadFromFile(create_info.model_file));
            }
        }
        std::unordered_map<std::string, VkBuffer> vertex_buffers;
        std::unordered_map<std::string, VkBuffer> index_buffers;
		std::unordered_map<std::string, size_t> index_counts;
        for (auto& model : models) {
            if (vertex_buffers.find(model.first) == vertex_buffers.end()) {
                std::cout << "Loading vertex buffer for " << model.first << std::endl;

				map.vertex_buffers.emplace_back(vulkan::Buffer{
					&device,
					VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					sizeof(vulkan::Vertex) * model.second.vertices.size()
				});

				auto& buffer = map.vertex_buffers.back();

				buffer.map().memcpy(model.second.vertices.data(), buffer.size);

				vertex_buffers.emplace(model.first, buffer);
            }
            if (index_buffers.find(model.first) == index_buffers.end()) {
                std::cout << "Loading index buffer for " << model.first << std::endl;

				map.index_buffers.emplace_back(vulkan::Buffer{
					&device,
					VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					sizeof(uint32_t) * model.second.indices.size()
				});

				auto& buffer = map.index_buffers.back();

				buffer.map().memcpy(model.second.indices.data(), buffer.size);

				index_buffers.emplace(model.first, buffer);
				index_counts.emplace(model.first, model.second.indices.size());
            }
        }

        // Preload shaders
        std::unordered_map<std::string, vulkan::ShaderModule> shader_modules;
        for (auto& material : materials) {
            for (auto shader : material.shaders) {
                if (shader_modules.find(shader.file) == shader_modules.end()) {
                    std::cout << "Loading shader " << shader.file << std::endl;
                    shader_modules.emplace(shader.file, vulkan::ShaderModule(device, shader.file));
                }
            }
        }

        // Descriptor set layout
        auto descriptor_set_layout = initDescriptorSetLayout(device);

		// Define hash function for descriptor sets
		auto descriptor_set_hash_function = [](const EntityCreateInfo& create_info) {
			size_t hash = 0;
			hash_combine(hash, create_info.texture_file);
			return hash;
		};

		// Calculate the number of unique descriptor sets we are going to create
		auto number_of_unique_descriptor_sets = [&]() {
			std::set<size_t> descriptor_set_hashes;
			for (auto& create_info : create_info_list) {
				descriptor_set_hashes.emplace(descriptor_set_hash_function(create_info));
			}
			return descriptor_set_hashes.size();
		}();

		// Create descriptor pool
		std::cout << "Creating descriptor pool" << std::endl;
		map.descriptor_pools.emplace_back(initDescriptorPool(device, number_of_unique_descriptor_sets));
		const auto& descriptor_pool = map.descriptor_pools.back();

        // Components
		map.components.emplace_back(new PhysicsComponent{});
        auto physics_component = map.components.back().get();

		// Define hash function for pipelines
		auto pipeline_hash_function = [](const EntityCreateInfo& create_info) {
			size_t hash = 0;
			for (const auto& shader : create_info.material->shaders) {
				hash_combine(hash, shader.entry_point);
				hash_combine(hash, shader.file);
				hash_combine(hash, shader.stage);
			}
			return hash;
		};

		// Calculate the number of unique pipelines we are going to create
		auto number_of_unique_pipelines = [&]() {
			std::set<size_t> pipeline_hashes;
			for (const auto& create_info : create_info_list) {
				pipeline_hashes.emplace(pipeline_hash_function(create_info));
			}
			return pipeline_hashes.size();
		}();

		// Iterate through all entities
		std::unordered_map<size_t, vulkan::Pipeline&> hashed_pipelines;
		std::unordered_map<size_t, VkDescriptorSet> hashed_descriptor_sets;
		for (auto& create_info : create_info_list) {
			// Get pipeline
			const auto& pipeline = [&]() -> const vulkan::Pipeline& {
				// Calculate hash
				size_t hash = pipeline_hash_function(create_info);

				if (hashed_pipelines.find(hash) == hashed_pipelines.end()) {
					std::cout << "Creating pipeline (" << std::to_string(hashed_pipelines.size() + 1) << "/" << std::to_string(number_of_unique_pipelines) << ")" << std::endl;

					// Shaders
					std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
					for (auto& shader : create_info.material->shaders) {
						shader_stages.emplace_back(vulkan::initializers::pipelineShaderStageCreateInfo(
							shader.stage,
							shader_modules.at(shader.file),
							shader.entry_point)
						);
					}

					// Create pipeline
					map.pipelines.emplace_back(vulkan::Pipeline{device, { descriptor_set_layout }, render_pass, extent, shader_stages});

					auto& pipeline = map.pipelines.back();
					hashed_pipelines.emplace(hash, pipeline);

					return pipeline;
				} else {
					return hashed_pipelines.at(hash);
				}
			}();

			// Get descriptor set
			auto descriptor_set = [&]() {
				// Calculate hash
				size_t hash = descriptor_set_hash_function(create_info);

				if (hashed_descriptor_sets.find(hash) == hashed_descriptor_sets.end()) {
					std::cout << "Allocating descriptor set (" << std::to_string(hashed_descriptor_sets.size() + 1) << "/" << std::to_string(number_of_unique_descriptor_sets) << ")" << std::endl;
					
					auto descriptor_set = hashed_descriptor_sets[hash] = descriptor_pool.allocateDescriptorSet(descriptor_set_layout);

					VkDescriptorImageInfo image_info = {};
					image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					image_info.imageView   = textures.at(create_info.texture_file).first;
					image_info.sampler     = textures.at(create_info.texture_file).second;
					
					std::vector<VkWriteDescriptorSet> write_descriptor_sets = {
						vulkan::initializers::writeDescriptorSet(descriptor_set, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &map.static_uniform_buffer.descriptor),
						vulkan::initializers::writeDescriptorSet(descriptor_set, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &map.dynamic_uniform_buffer.descriptor),
						vulkan::initializers::writeDescriptorSet(descriptor_set, 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &image_info)
					};
					vkUpdateDescriptorSets(device, write_descriptor_sets.size(), write_descriptor_sets.data(), 0, nullptr);
					
					return descriptor_set;
				} else {
					return hashed_descriptor_sets.at(hash);
				}
			}();

            // Transform (Dynamic uniform buffer)
            uint32_t dynamic_offset = next_dynamic_index++ * static_cast<uint32_t>(dynamic_alignment);
            auto transform = (glm::mat4*)(((uint64_t)map.dynamic_uniform_buffer_object.model + dynamic_offset));

            // Render instruction
			RenderInstruction render_instruction = {};
            render_instruction.pipeline_layout       = pipeline.layout;
            render_instruction.pipeline              = pipeline;
            render_instruction.vertex_buffers        = { vertex_buffers.at(create_info.model_file) };
            render_instruction.vertex_buffer_offsets = { 0 };
            render_instruction.index_buffer          = index_buffers.at(create_info.model_file);
			render_instruction.index_count           = index_counts.at(create_info.model_file);
            render_instruction.descriptor_sets       = { descriptor_set };
            render_instruction.dynamic_offsets       = { dynamic_offset };
            map.render_instructions.emplace_back(render_instruction);

            // Compose entity
            map.entities.emplace_back(Entity{transform});
            auto& entity = map.entities.back();

            entity.transform().moveTo(create_info.position);
            entity.transform().setRotation(create_info.rotation);
            entity.transform().setScale(create_info.scale);

            entity.addComponent(physics_component);
        }

		std::cout << "Map instantiation complete" << std::endl;
		
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

	Map::Map(vulkan::Buffer&& static_uniform_buffer, vulkan::Buffer&& dynamic_uniform_buffer) :
		static_uniform_buffer          (std::move(static_uniform_buffer)),
		dynamic_uniform_buffer         (std::move(dynamic_uniform_buffer)),
		static_uniform_buffer_mapping  (this->static_uniform_buffer.map()),
		dynamic_uniform_buffer_mapping (this->dynamic_uniform_buffer.map()) {
	}

}