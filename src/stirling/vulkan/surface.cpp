#include "surface.h"

namespace stirling {

	VulkanSurface::VulkanSurface(VkSurfaceKHR surface) :
		m_surface (surface) {
	}

	VulkanSurface::operator VkSurfaceKHR() const {
		return m_surface;
	}

}