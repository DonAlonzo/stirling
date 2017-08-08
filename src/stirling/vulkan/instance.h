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
            std::vector<VkLayerProperties> layer_properties;
            std::vector<PhysicalDevice>    physical_devices;

            Instance(const std::vector<const char*>& extensions);

        private:
            Validator validator;

            VkInstance                     init(std::vector<const char*> extensions) const;
            Validator                      initValidator() const;
            std::vector<VkLayerProperties> getLayerProperties() const;
            std::vector<PhysicalDevice>    getPhysicalDevices() const;

            bool checkValidationLayerSupport() const;
        };

    }
}