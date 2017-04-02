#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class CommandBuffer {
		public:
			CommandBuffer(const VkCommandBuffer& command_buffer);

			operator VkCommandBuffer() const;

		private:
			VkCommandBuffer m_command_buffer;
		};
	}
}