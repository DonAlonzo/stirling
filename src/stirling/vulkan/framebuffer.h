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
		class Framebuffer {
		public:
			Framebuffer(const Device& device, VkFramebuffer);
			~Framebuffer();
			Framebuffer(Framebuffer&&) = default;
			Framebuffer(const Framebuffer&) = delete;
			Framebuffer& operator=(Framebuffer&&) = default;
			Framebuffer& operator=(const Framebuffer&) = delete;

		private:
			const Device& m_device;

			VkFramebuffer m_framebuffer;
		};
	}
}