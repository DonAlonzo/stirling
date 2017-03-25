#include "framebuffer.h"

#include "device.h"

namespace stirling {
	namespace vulkan {
		
		Framebuffer::Framebuffer(const Device& device, VkFramebuffer framebuffer) :
			m_device      (device),
			m_framebuffer (framebuffer) {
		}

		Framebuffer::~Framebuffer() {
			vkDestroyFramebuffer(m_device, m_framebuffer, nullptr);
		}
		
	}
}