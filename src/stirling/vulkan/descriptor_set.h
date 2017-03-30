#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

namespace stirling {
	namespace vulkan {
		class DescriptorSet {
		public:
			DescriptorSet(const Device& device, VkDescriptorSet descriptor_set);
			~DescriptorSet() = default;
			DescriptorSet(DescriptorSet&&) = default;
			DescriptorSet(const DescriptorSet&) = delete;
			DescriptorSet& operator=(DescriptorSet&&) = delete;
			DescriptorSet& operator=(const DescriptorSet&) = delete;

			operator VkDescriptorSet() const;

		private:
			const Device&   m_device;
			VkDescriptorSet m_descriptor_set;
		};
	}
}