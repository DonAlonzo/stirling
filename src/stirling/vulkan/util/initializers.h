#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        namespace initializers {

            inline VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet         dst_set,
                                                           VkDescriptorType        type,
                                                           uint32_t                binding, 
                                                           VkDescriptorBufferInfo* buffer_info,
                                                           uint32_t                descriptor_count = 1) {
                VkWriteDescriptorSet write_descriptor_set = {};
                write_descriptor_set.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write_descriptor_set.dstSet          = dst_set;
                write_descriptor_set.descriptorType  = type;
                write_descriptor_set.dstBinding      = binding;
                write_descriptor_set.pBufferInfo     = buffer_info;
                write_descriptor_set.descriptorCount = descriptor_count;
                return write_descriptor_set;
            }

            inline VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet        dst_set,
                                                           VkDescriptorType       type,
                                                           uint32_t               binding,
                                                           VkDescriptorImageInfo* image_info,
                                                           uint32_t               descriptor_count = 1) {
                VkWriteDescriptorSet write_descriptor_set = {};
                write_descriptor_set.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write_descriptor_set.dstSet          = dst_set;
                write_descriptor_set.descriptorType  = type;
                write_descriptor_set.dstBinding      = binding;
                write_descriptor_set.pImageInfo      = image_info;
                write_descriptor_set.descriptorCount = descriptor_count;
                return write_descriptor_set;
            }

        }
    }
}