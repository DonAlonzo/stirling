#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

#include <vector>

namespace stirling {
	namespace vulkan {
		class CommandPool {
		public:
			CommandPool(const Device& device, int queue_family_index);
			~CommandPool();
			CommandPool(CommandPool&&) = default;
			CommandPool(const CommandPool&) = delete;
			CommandPool& operator=(CommandPool&&) = delete;
			CommandPool& operator=(const CommandPool&) = delete;

			operator VkCommandPool() const;

			std::vector<VkCommandBuffer> allocateCommandBuffers(uint32_t count) const;

		private:
			const Device& m_device;

			VkCommandPool m_command_pool;

			VkCommandPool initCommandPool(int queue_family_index) const;
		};
	}
}