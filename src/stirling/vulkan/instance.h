#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "validator.h"
namespace stirling { namespace vulkan {
    class PhysicalDevice;
}}
#include "deleter.hpp"

// std
#include <vector>

namespace stirling {
    namespace vulkan {
        class Instance {
        public:
            Instance(const std::vector<const char*>& extensions);

            operator VkInstance() const;

            std::vector<VkLayerProperties> getLayerProperties() const;
            std::vector<PhysicalDevice> getPhysicalDevices() const;

        private:
            Deleter<VkInstance> m_instance;
            Validator           m_validator;

            Deleter<VkInstance> initInstance(std::vector<const char*> extensions) const;
            Validator           initValidator() const;

            bool checkValidationLayerSupport() const;
        };
    }
}