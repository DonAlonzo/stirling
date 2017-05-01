#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
	class Instance;
}}
#include "deleter.h"

namespace stirling {
	namespace vulkan {

		class Surface {
		public:
			Surface(const Instance& instance, VkSurfaceKHR);

			operator VkSurfaceKHR() const;

		private:
			Deleter<VkSurfaceKHR> m_surface;
		};

	}
}