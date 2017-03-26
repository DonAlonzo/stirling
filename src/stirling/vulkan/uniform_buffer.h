#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}
#include "uniform_buffer_object.h"
#include "buffer.h"

namespace stirling {
	namespace vulkan {
		class UniformBuffer : public Buffer {
		public:
			UniformBuffer(const Device& device);

			void update(const UniformBufferObject& uniform_buffer_object) const;
		};
	}
}