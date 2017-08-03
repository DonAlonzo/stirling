#include "descriptor_pool.h"

namespace stirling {
    namespace vulkan {

        DescriptorPool::DescriptorPool(VkDevice device, const std::vector<VkDescriptorPoolSize>& pool_sizes, uint32_t max_sets) :
            Deleter<VkDescriptorPool>(init(device, pool_sizes, max_sets), device, vkDestroyDescriptorPool),
            device (device) {
        }

        VkDescriptorPool DescriptorPool::init(VkDevice device, const std::vector<VkDescriptorPoolSize>& pool_sizes, uint32_t max_sets) const {
            VkDescriptorPoolCreateInfo create_info = {};
            create_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            create_info.poolSizeCount = pool_sizes.size();
            create_info.pPoolSizes    = pool_sizes.data();
            create_info.maxSets       = max_sets;

            VkDescriptorPool descriptor_pool;
            if (vkCreateDescriptorPool(device, &create_info, nullptr, &descriptor_pool) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create descriptor pool.");
            }
            return descriptor_pool;
        }

        VkDescriptorSet DescriptorPool::allocateDescriptorSet(const VkDescriptorSetLayout& set_layout) const {
            VkDescriptorSetAllocateInfo allocate_info = {};
            allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocate_info.descriptorPool     = *this;
            allocate_info.descriptorSetCount = 1;
            allocate_info.pSetLayouts        = &set_layout;

            VkDescriptorSet descriptor_set;
            if (vkAllocateDescriptorSets(device, &allocate_info, &descriptor_set) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate descriptor sets.");
            }
            return descriptor_set;
        }

    }
}