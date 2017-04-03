#include "sampler.h"

#include "device.h"

namespace stirling {
	namespace vulkan {

		Sampler::Sampler(const Device& device, VkSampler&& sampler) :
			m_device  (&device),
			m_sampler (std::move(sampler)) {
		}

		Sampler::Sampler(const Device& device, const VkSamplerCreateInfo& create_info) :
			m_device  (&device),
			m_sampler (createSampler(create_info)) {
		}

		VkSampler Sampler::createSampler(const VkSamplerCreateInfo& create_info) {
			VkSampler sampler;
			if (vkCreateSampler(*m_device, &create_info, nullptr, &sampler) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create texture sampler.");
			}
			return sampler;
		}

		Sampler::Sampler(Sampler&& rhs) :
			m_device  (std::move(rhs.m_device)),
			m_sampler (std::move(rhs.m_sampler)) {
			rhs.m_sampler = VK_NULL_HANDLE;
		}

		Sampler::~Sampler() {
			if (m_sampler != VK_NULL_HANDLE) {
				vkDestroySampler(*m_device, m_sampler, nullptr);
			}
		}

		Sampler::operator VkSampler() const {
			return m_sampler;
		}

	}
}