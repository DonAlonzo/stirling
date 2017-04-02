#include "command_buffer.h"

namespace stirling {
	namespace vulkan {

		CommandBuffer::CommandBuffer(const VkCommandBuffer& command_buffer) :
			m_command_buffer (command_buffer) {
		}

		CommandBuffer::operator VkCommandBuffer() const {
			return m_command_buffer;
		}

	}
}