#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

namespace stirling {
	namespace vulkan {

		class Queue {
		public:
			Queue(const Device& device, VkQueue queue);

		private:
			const Device& m_device;

			VkQueue             m_queue;
		};

	}
}