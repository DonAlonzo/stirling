#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        namespace initializers {
            
            inline VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptor_type, VkShaderStageFlags stage_flags, uint32_t descriptor_count = 1) {
                VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {};
                descriptor_set_layout_binding.binding         = binding;
                descriptor_set_layout_binding.descriptorType  = descriptor_type;
                descriptor_set_layout_binding.descriptorCount = descriptor_count;
                descriptor_set_layout_binding.stageFlags      = stage_flags;
                return descriptor_set_layout_binding;
            }

            inline VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet dst_set, uint32_t binding, VkDescriptorType type, const VkDescriptorBufferInfo* buffer_info, uint32_t descriptor_count = 1) {
                VkWriteDescriptorSet write_descriptor_set = {};
                write_descriptor_set.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write_descriptor_set.dstSet          = dst_set;
                write_descriptor_set.descriptorType  = type;
                write_descriptor_set.dstBinding      = binding;
                write_descriptor_set.pBufferInfo     = buffer_info;
                write_descriptor_set.descriptorCount = descriptor_count;
                return write_descriptor_set;
            }

            inline VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet dst_set, uint32_t binding, VkDescriptorType type, const VkDescriptorImageInfo* image_info, uint32_t descriptor_count = 1) {
                VkWriteDescriptorSet write_descriptor_set = {};
                write_descriptor_set.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write_descriptor_set.dstSet          = dst_set;
                write_descriptor_set.descriptorType  = type;
                write_descriptor_set.dstBinding      = binding;
                write_descriptor_set.pImageInfo      = image_info;
                write_descriptor_set.descriptorCount = descriptor_count;
                return write_descriptor_set;
            }

            inline VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule module, const char* name) {
                VkPipelineShaderStageCreateInfo create_info = {};
                create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                create_info.stage  = stage;
                create_info.module = module;
                create_info.pName  = name;
                return create_info;
            }


        }
    }
}