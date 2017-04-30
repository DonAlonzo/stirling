#include "sampler.h"

#include "device.h"

namespace stirling {
    namespace vulkan {

        Sampler::Sampler(const Device& device, VkSampler sampler) :
            m_sampler (Deleter<VkSampler>(sampler, device, vkDestroySampler)) {
        }

        Sampler::Sampler(const Device& device, const VkSamplerCreateInfo& create_info) :
            m_sampler (createSampler(device, create_info)) {
        }

        Deleter<VkSampler> Sampler::createSampler(const Device& device, const VkSamplerCreateInfo& create_info) const {
            VkSampler sampler;
            if (vkCreateSampler(device, &create_info, nullptr, &sampler) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create texture sampler.");
            }
            return Deleter<VkSampler>(sampler, device, vkDestroySampler);
        }

        Sampler::operator const VkSampler&() const {
            return m_sampler;
        }

    }
}