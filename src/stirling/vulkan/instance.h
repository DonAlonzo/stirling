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
	
		VulkanInstance(const VulkanInstance& rhs) = delete;
		VulkanInstance& operator=(const VulkanInstance& rhs) = delete;

		VulkanInstance(VulkanInstance&& rhs) = default;
		VulkanInstance& operator=(VulkanInstance&& rhs) = default;

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