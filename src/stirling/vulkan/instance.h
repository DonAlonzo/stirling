#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
#include "validator.h"
namespace stirling {
	class VulkanPhysicalDevice;
}

// std
#include <vector>

namespace stirling {

	class VulkanInstance {
	public:
		VulkanInstance(const std::vector<const char*>& extensions);
		~VulkanInstance();
		VulkanInstance(VulkanInstance&&) = default;
		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(VulkanInstance&&) = default;
		VulkanInstance& operator=(const VulkanInstance&) = delete;

		operator VkInstance() const;

		std::vector<VkLayerProperties> getLayerProperties() const;
		std::vector<VulkanPhysicalDevice> getPhysicalDevices() const;

	private:
		VkInstance      m_instance;
		VulkanValidator m_validator;

		VkInstance      initInstance(std::vector<const char*> extensions) const;
		VulkanValidator initValidator() const;
		
		bool checkValidationLayerSupport() const;
	};

}