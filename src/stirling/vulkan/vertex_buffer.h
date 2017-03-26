#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	struct Vertex;
	namespace vulkan {
		class Device;
	}
}
#include "buffer.h"

#include <vector>

namespace stirling {
	namespace vulkan {
		class VertexBuffer : public Buffer {
		public:
			VertexBuffer(const Device& device, const std::vector<Vertex>& vertices);

			int size() const;

		private:
			int m_size;
		};
	}
}