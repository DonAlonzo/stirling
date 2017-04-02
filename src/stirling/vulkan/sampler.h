#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

namespace stirling {
	namespace vulkan {
		class Sampler {
		public:
			Sampler(const Device& device, VkSampler&& sampler);
			Sampler(const Device& device, const VkSamplerCreateInfo& create_info);
			~Sampler();
			Sampler(Sampler&& rhs);
			Sampler(const Sampler&) = delete;
			Sampler& operator=(Sampler&&) = delete;
			Sampler& operator=(const Sampler&) = delete;

			operator VkSampler() const;

		private:
			const Device& m_device;
			VkSampler     m_sampler;

			VkSampler     createSampler(const VkSamplerCreateInfo& create_info);
		};
	}
}