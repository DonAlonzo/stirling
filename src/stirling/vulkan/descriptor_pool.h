#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

#include <vector>

namespace stirling {
	namespace vulkan {
		class DescriptorPool {
		public:
			DescriptorPool(const Device& device, const std::vector<VkDescriptorPoolSize>& pool_sizes, int max_sets);
			~DescriptorPool();
			DescriptorPool(DescriptorPool&&) = default;
			DescriptorPool(const DescriptorPool&) = delete;
			DescriptorPool& operator=(DescriptorPool&&) = delete;
			DescriptorPool& operator=(const DescriptorPool&) = delete;

			operator VkDescriptorPool() const;

			std::vector<VkDescriptorSet> allocateDescriptorSets(const std::vector<VkDescriptorSetLayout>& set_layouts) const;

		private:
			const Device& m_device;

			VkDescriptorPool m_descriptor_pool;

			VkDescriptorPool initDescriptorPool(const std::vector<VkDescriptorPoolSize>& pool_sizes, int max_sets) const;
		};
	}
}