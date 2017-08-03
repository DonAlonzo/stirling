#pragma once

#include "vulkan/vulkan.h"

#include "deleter.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        struct DescriptorPool : Deleter<VkDescriptorPool> {
            DescriptorPool(VkDevice device, const std::vector<VkDescriptorPoolSize>& pool_sizes, uint32_t max_sets);

            VkDescriptorSet allocateDescriptorSet(const VkDescriptorSetLayout& set_layout) const;

        private:
            VkDevice device;

            VkDescriptorPool init(VkDevice device, const std::vector<VkDescriptorPoolSize>& pool_sizes, uint32_t max_sets) const;
        };
    }
}