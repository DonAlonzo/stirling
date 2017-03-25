#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {

		class Surface {
		public:
			Surface(VkSurfaceKHR);

			operator VkSurfaceKHR() const;

		private:
			VkSurfaceKHR m_surface;
		};

	}
}