#include "surface.h"
#include "instance.h"

namespace stirling {
	namespace vulkan {

		Surface::Surface(const Instance& instance, VkSurfaceKHR surface) :
			m_instance (instance),
			m_surface  (surface) {
		}

		Surface::Surface(Surface&& rhs) :
			m_instance (std::move(rhs.m_instance)),
			m_surface  (std::move(rhs.m_surface)) {
			rhs.m_surface = VK_NULL_HANDLE;
		}

		Surface::~Surface() {
			if (m_surface != VK_NULL_HANDLE) {
				vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
			}
		}

		Surface::operator VkSurfaceKHR() const {
			return m_surface;
		}

	}
}