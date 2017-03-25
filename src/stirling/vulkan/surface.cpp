#include "surface.h"

namespace stirling {
	namespace vulkan {

		Surface::Surface(VkSurfaceKHR surface) :
			m_surface(surface) {
		}

		Surface::operator VkSurfaceKHR() const {
			return m_surface;
		}

	}
}