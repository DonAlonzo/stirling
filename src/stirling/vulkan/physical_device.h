#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// Stirling
namespace stirling {
	namespace vulkan {
		class Surface;
	}
}
#include "device.h"

// std
#include <vector>

namespace stirling {
	namespace vulkan {

		struct QueueFamilyIndices {
			int graphics_family_index;
			int present_family_index;

			bool isComplete() const;
		};

		class PhysicalDevice {
		public:
			PhysicalDevice(VkPhysicalDevice physical_device);
		
			operator VkPhysicalDevice() const;

			QueueFamilyIndices findQueueFamilies(const Surface& surface) const;

			Device createDevice(const Surface& surface, const std::vector<const char*> extensions) const;

			std::vector<VkExtensionProperties> getExtensions() const;
			VkSurfaceCapabilitiesKHR getSurfaceCapabilities(const Surface& surface) const;
			std::vector<VkSurfaceFormatKHR> getSurfaceFormats(const Surface& surface) const;
			std::vector<VkPresentModeKHR> getSurfacePresentModes(const Surface& surface) const;

		private:
			VkPhysicalDevice m_physical_device;
		};

	}
}