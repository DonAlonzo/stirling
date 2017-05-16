#pragma once

#include "vulkan/vulkan.h"

#include "deleter.h"

#include <vector>

namespace stirling {
    namespace vulkan {
        class DescriptorPool {
        public:
            DescriptorPool(VkDevice device, const std::vector<VkDescriptorPoolSize>& pool_sizes, uint32_t max_sets);

            operator VkDescriptorPool() const;

            VkDescriptorSet              allocateDescriptorSet(const VkDescriptorSetLayout& set_layout) const;
            std::vector<VkDescriptorSet> allocateDescriptorSets(const std::vector<VkDescriptorSetLayout>& set_layouts) const;

        private:
            VkDevice                  m_device;
            Deleter<VkDescriptorPool> m_descriptor_pool;

            Deleter<VkDescriptorPool> initDescriptorPool(const std::vector<VkDescriptorPoolSize>& pool_sizes, uint32_t max_sets) const;
        };
    }
}