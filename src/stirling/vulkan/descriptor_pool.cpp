#include "descriptor_pool.h"
#include "device.h"

namespace stirling {
	namespace vulkan {

		DescriptorPool::DescriptorPool(const Device& device, const std::vector<VkDescriptorPoolSize>& pool_sizes, int max_sets) :
			m_device          (device),
			m_descriptor_pool (initDescriptorPool(pool_sizes, max_sets)) {
		}

		VkDescriptorPool DescriptorPool::initDescriptorPool(const std::vector<VkDescriptorPoolSize>& pool_sizes, int max_sets) const {
			VkDescriptorPoolCreateInfo create_info = {};
			create_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			create_info.poolSizeCount = pool_sizes.size();
			create_info.pPoolSizes    = pool_sizes.data();
			create_info.maxSets       = max_sets;

			VkDescriptorPool descriptor_pool;
			if (vkCreateDescriptorPool(m_device, &create_info, nullptr, &descriptor_pool) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create descriptor pool.");
			}
			return descriptor_pool;
		}

		DescriptorPool::~DescriptorPool() {
			vkDestroyDescriptorPool(m_device, m_descriptor_pool, nullptr);
		}

		DescriptorPool::operator VkDescriptorPool() const {
			return m_descriptor_pool;
		}

		std::vector<VkDescriptorSet> DescriptorPool::allocateDescriptorSets(const std::vector<VkDescriptorSetLayout>& set_layouts) const {
			VkDescriptorSetAllocateInfo allocate_info = {};
			allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocate_info.descriptorPool     = m_descriptor_pool;
			allocate_info.descriptorSetCount = set_layouts.size();
			allocate_info.pSetLayouts        = set_layouts.data();

			std::vector<VkDescriptorSet> descriptor_sets;
			if (vkAllocateDescriptorSets(m_device, &allocate_info, descriptor_sets.data()) != VK_SUCCESS) {
				throw std::runtime_error("Failed to allocate descriptor sets.");
			}
			return descriptor_sets;
		}

	}
}