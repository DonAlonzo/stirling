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
			Framebuffer(Framebuffer&&);
			Framebuffer(const Framebuffer&) = delete;
			Framebuffer& operator=(Framebuffer&&) = delete;
			Framebuffer& operator=(const Framebuffer&) = delete;

			operator VkFramebuffer() const;

		private:
			const Device& m_device;

			VkFramebuffer m_framebuffer;
		};
	}
}