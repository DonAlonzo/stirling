#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "validator.h"
namespace stirling {
	namespace vulkan {
		class PhysicalDevice;
	}
}

// std
#include <vector>

namespace stirling {
	namespace vulkan {
		class Instance {
		public:
			Instance(const std::vector<const char*>& extensions);
			~Instance();
			Instance(Instance&&) = default;
			Instance(const Instance&) = delete;
			Instance& operator=(Instance&&) = default;
			Instance& operator=(const Instance&) = delete;

			operator VkInstance() const;

			std::vector<VkLayerProperties> getLayerProperties() const;
			std::vector<PhysicalDevice> getPhysicalDevices() const;

		private:
			VkInstance m_instance;
			Validator  m_validator;

			VkInstance initInstance(std::vector<const char*> extensions) const;
			Validator  initValidator() const;

			bool checkValidationLayerSupport() const;
		};
	}
}