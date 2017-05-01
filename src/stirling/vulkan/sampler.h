#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan { 
    class Device;
}}
#include "deleter.h"

namespace stirling {
    namespace vulkan {
        class Sampler {
        public:
            Sampler(const Device& device, VkSampler sampler);
            Sampler(const Device& device, const VkSamplerCreateInfo& create_info);

            operator const VkSampler&() const;

        private:
            Deleter<VkSampler> m_sampler;

            Deleter<VkSampler> createSampler(const Device& device, const VkSamplerCreateInfo& create_info) const;
        };
    }
}