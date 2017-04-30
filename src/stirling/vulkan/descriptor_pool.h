#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
    namespace vulkan {
        class Device;
    }
}
#include "deleter.hpp"

#include <vector>

namespace stirling {
    namespace vulkan {
        class DescriptorPool {
        public:
            DescriptorPool(const Device& device, const std::vector<VkDescriptorPoolSize>& pool_sizes, int max_sets);

            operator VkDescriptorPool() const;

            VkDescriptorSet              allocateDescriptorSet(const VkDescriptorSetLayout& set_layout) const;
            std::vector<VkDescriptorSet> allocateDescriptorSets(const std::vector<VkDescriptorSetLayout>& set_layouts) const;

        private:
            const Device&             m_device;
            Deleter<VkDescriptorPool> m_descriptor_pool;

            Deleter<VkDescriptorPool> initDescriptorPool(const std::vector<VkDescriptorPoolSize>& pool_sizes, int max_sets) const;
        };
    }
}