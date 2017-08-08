#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "validator.h"
namespace stirling { namespace vulkan {
    struct PhysicalDevice;
}}
#include "deleter.h"

// std
#include <vector>

namespace stirling {
    namespace vulkan {

        struct Instance : Deleter<VkInstance> {
            Instance(const std::vector<const char*>& extensions);

            std::vector<VkLayerProperties> getLayerProperties() const;
            std::vector<PhysicalDevice> getPhysicalDevices() const;

        private:
            Validator m_validator;

            VkInstance init(std::vector<const char*> extensions) const;
            Validator  initValidator() const;

            bool checkValidationLayerSupport() const;
        };

    }
}