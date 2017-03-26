#pragma once

#include "vulkan/vulkan.h"

#include <glm/glm.hpp>

#include <array>

namespace stirling {
	struct Vertex {
		glm::vec2 position;
		glm::vec3 color;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription binding_description = {};
			binding_description.binding   = 0;
			binding_description.stride    = sizeof(Vertex);
			binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return binding_description;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions = {};
			attribute_descriptions[0].binding  = 0;
			attribute_descriptions[0].location = 0;
			attribute_descriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
			attribute_descriptions[0].offset   = offsetof(Vertex, position);

			attribute_descriptions[1].binding  = 0;
			attribute_descriptions[1].location = 1;
			attribute_descriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
			attribute_descriptions[1].offset   = offsetof(Vertex, color);
			return attribute_descriptions;
		}
	};
}

// stirling::vulkan
#include "vulkan/command_pool.h"
#include "vulkan/device.h"
#include "vulkan/index_buffer.h"
#include "vulkan/instance.h"
#include "vulkan/pipeline.h"
#include "vulkan/physical_device.h"
#include "vulkan/render_pass.h"
#include "vulkan/semaphore.h"
#include "vulkan/surface.h"
#include "vulkan/swapchain.h"
#include "vulkan/vertex_buffer.h"