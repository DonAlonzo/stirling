#include "framebuffer.h"

#include "device.h"

namespace stirling {
	namespace vulkan {
		
		Framebuffer::Framebuffer(const Device& device, VkFramebuffer framebuffer) :
			m_device      (&device),
			m_framebuffer (framebuffer) {
		}

		Framebuffer::Framebuffer(Framebuffer&& rhs) :
			m_device      (std::move(rhs.m_device)),
			m_framebuffer (std::move(rhs.m_framebuffer)) {
			rhs.m_framebuffer = VK_NULL_HANDLE;
		}

		Framebuffer::~Framebuffer() {
			if (m_framebuffer != VK_NULL_HANDLE) {
				vkDestroyFramebuffer(*m_device, m_framebuffer, nullptr);
			}
		}

		Framebuffer::operator VkFramebuffer() const {
			return m_framebuffer;
		}
		
	}
}