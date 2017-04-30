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
			Queue(Device* device, int family_index);

			operator const VkQueue&() const;

			CommandPool createCommandPool() const;

		private:
			Device* m_device;
			int     m_family_index;
			VkQueue m_queue;

			VkQueue initQueue() const;
		};

	}
}