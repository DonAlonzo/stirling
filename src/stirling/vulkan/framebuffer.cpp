#include "framebuffer.h"

#include "device.h"

namespace stirling {
	namespace vulkan {
		
		Framebuffer::Framebuffer(const Device& device, VkFramebuffer framebuffer) :
			m_framebuffer (Deleter<VkFramebuffer>(framebuffer, device, vkDestroyFramebuffer)) {
		}

		Framebuffer::operator VkFramebuffer() const {
			return m_framebuffer;
		}
		
	}
}