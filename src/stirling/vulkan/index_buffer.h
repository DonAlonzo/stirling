#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}
#include "buffer.h"

#include <vector>

namespace stirling {
	namespace vulkan {
		class IndexBuffer : public Buffer {
		public:
			IndexBuffer(const Device& device, const std::vector<uint16_t>& indices);

			int size() const;

		private:
			int m_size;
		};
	}
}