#include "pipeline.h"
#include "device.h"
#include "render_pass.h"

namespace stirling {
	namespace vulkan {

		Pipeline::Pipeline(const Device& device, const RenderPass& render_pass, const VkExtent2D& extent) :
			m_device          (device),
			m_pipeline_layout (initPipelineLayout()),
			m_pipeline        (initPipeline(render_pass, extent)) {
		}

		void Pipeline::reset(const RenderPass& render_pass, const VkExtent2D& extent) {
			vkDestroyPipeline(m_device, m_pipeline, nullptr);
			m_pipeline = initPipeline(render_pass, extent);
		}

		VkPipelineLayout Pipeline::initPipelineLayout() const {
			VkPipelineLayoutCreateInfo pipeline_layout_info = {};
			pipeline_layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipeline_layout_info.setLayoutCount         = 0;
			pipeline_layout_info.pSetLayouts            = nullptr;
			pipeline_layout_info.pushConstantRangeCount = 0;
			pipeline_layout_info.pPushConstantRanges    = 0;

			VkPipelineLayout pipeline_layout;
			if (vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create pipeline layout.");
			}
			return pipeline_layout;
		}

		VkPipeline Pipeline::initPipeline(const RenderPass& render_pass, const VkExtent2D& extent) const {
			ShaderModule vertex_shader(*this, "shaders/vert.spv");
			ShaderModule fragment_shader(*this, "shaders/frag.spv");

			VkPipelineShaderStageCreateInfo vertex_shader_stage_info = {};
			vertex_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertex_shader_stage_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
			vertex_shader_stage_info.module = vertex_shader;
			vertex_shader_stage_info.pName  = "main";

			VkPipelineShaderStageCreateInfo fragment_shader_stage_info = {};
			fragment_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragment_shader_stage_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragment_shader_stage_info.module = fragment_shader;
			fragment_shader_stage_info.pName  = "main";

			VkPipelineShaderStageCreateInfo shader_stages[] = {
				vertex_shader_stage_info,
				fragment_shader_stage_info
			};

			VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
			vertex_input_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertex_input_info.vertexBindingDescriptionCount   = 0;
			vertex_input_info.pVertexBindingDescriptions      = nullptr;
			vertex_input_info.vertexAttributeDescriptionCount = 0;
			vertex_input_info.pVertexAttributeDescriptions    = nullptr;

			VkPipelineInputAssemblyStateCreateInfo input_assembly = {};
			input_assembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			input_assembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			input_assembly.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport = {};
			viewport.x        = 0.0f;
			viewport.y        = 0.0f;
			viewport.width    = (float)extent.width;
			viewport.height   = (float)extent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = extent;

			VkPipelineViewportStateCreateInfo viewport_state = {};
			viewport_state.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewport_state.viewportCount = 1;
			viewport_state.pViewports    = &viewport;
			viewport_state.scissorCount  = 1;
			viewport_state.pScissors     = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable        = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth               = 1.0f;
			rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable         = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f;
			rasterizer.depthBiasClamp          = 0.0f;
			rasterizer.depthBiasSlopeFactor    = 0.0f;

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable  = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			// TODO: Depth and stencil testing

			VkPipelineColorBlendAttachmentState color_blend_attachment = {};
			color_blend_attachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			color_blend_attachment.blendEnable         = VK_FALSE;
			color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			color_blend_attachment.colorBlendOp        = VK_BLEND_OP_ADD;
			color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			color_blend_attachment.alphaBlendOp        = VK_BLEND_OP_ADD;

			VkPipelineColorBlendStateCreateInfo color_blending = {};
			color_blending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			color_blending.logicOpEnable     = VK_FALSE;
			color_blending.logicOp           = VK_LOGIC_OP_COPY;
			color_blending.attachmentCount   = 1;
			color_blending.pAttachments      = &color_blend_attachment;
			color_blending.blendConstants[0] = 0.0f;
			color_blending.blendConstants[1] = 0.0f;
			color_blending.blendConstants[2] = 0.0f;
			color_blending.blendConstants[3] = 0.0f;

			VkGraphicsPipelineCreateInfo create_info = {};
			create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			create_info.stageCount          = 2;
			create_info.pStages             = shader_stages;
			create_info.pVertexInputState   = &vertex_input_info;
			create_info.pInputAssemblyState = &input_assembly;
			create_info.pViewportState      = &viewport_state;
			create_info.pRasterizationState = &rasterizer;
			create_info.pMultisampleState   = &multisampling;
			create_info.pDepthStencilState  = nullptr;
			create_info.pColorBlendState    = &color_blending;
			create_info.pDynamicState       = nullptr;
			create_info.layout              = m_pipeline_layout;
			create_info.renderPass          = render_pass;
			create_info.subpass             = 0;
			create_info.basePipelineHandle  = VK_NULL_HANDLE;
			create_info.basePipelineIndex   = -1;

			VkPipeline pipeline;
			if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create graphics pipeline.");
			}
			return pipeline;
		}

		Pipeline::~Pipeline() {
			vkDestroyPipeline(m_device, m_pipeline, nullptr);
			vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
		}

		Pipeline::operator VkPipeline() const {
			return m_pipeline;
		}

		const Device& Pipeline::getDevice() const {
			return m_device;
		}

	}
}