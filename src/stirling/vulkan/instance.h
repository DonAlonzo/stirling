#pragma once

#include "vulkan/vulkan.h"

#include "validator.h"

#include <vector>

namespace stirling {

	class VulkanPhysicalDevice;

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