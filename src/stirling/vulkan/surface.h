#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Instance;
	}
}

namespace stirling {
	namespace vulkan {

		class Surface {
		public:
			Surface(const Instance& instance, VkSurfaceKHR);
			~Surface();
			Surface(Surface&&);
			Surface(const Surface&) = delete;
			Surface& operator=(Surface&&) = delete;
			Surface& operator=(const Surface&) = delete;

			operator VkSurfaceKHR() const;

		private:
			const Instance* m_instance;
			VkSurfaceKHR    m_surface;
		};

	}
}