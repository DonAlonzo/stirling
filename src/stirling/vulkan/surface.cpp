#include "surface.h"
#include "instance.h"

namespace stirling {
	namespace vulkan {

		Surface::Surface(const Instance& instance, VkSurfaceKHR surface) :
			m_surface (Deleter<VkSurfaceKHR>(surface, instance, vkDestroySurfaceKHR)) {
		}

		Surface::operator VkSurfaceKHR() const {
			return m_surface;
		}

	}
}