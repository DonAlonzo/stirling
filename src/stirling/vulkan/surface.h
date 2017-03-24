#pragma once

#include "vulkan/vulkan.h"

namespace stirling {

	class VulkanSurface {
	public:
		VulkanSurface(VkSurfaceKHR);

		operator VkSurfaceKHR() const;

	private:
		VkSurfaceKHR m_surface;
	};

}