#include "descriptor_set.h"

#include "device.h"

namespace stirling {
	namespace vulkan {

		DescriptorSet::DescriptorSet(const Device& device, VkDescriptorSet descriptor_set) :
			m_device         (device),
			m_descriptor_set (descriptor_set) {
		}

		DescriptorSet::operator VkDescriptorSet() const {
			return m_descriptor_set;
		}

	}
}