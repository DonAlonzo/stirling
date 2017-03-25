#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class CommandPool;
		class Device;
	}
}

namespace stirling {
	namespace vulkan {

		class Queue {
		public:
			Queue(const Device& device, int family_index);

			CommandPool createCommandPool() const;

		private:
			const Device& m_device;

			int           m_family_index;
			VkQueue       m_queue;

			VkQueue       initQueue() const;
		};

	}
}