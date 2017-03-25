#include "semaphore.h"
#include "device.h"

namespace stirling {
	namespace vulkan {

		Semaphore::Semaphore(const Device& device) :
			m_device (device) {
		}

		VkSemaphore Semaphore::initSemaphore() const {
			VkSemaphoreCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkSemaphore semaphore;
			if (vkCreateSemaphore(m_device, &create_info, nullptr, &semaphore) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create semaphore.");
			}
			return semaphore;
		}

		Semaphore::Semaphore(Semaphore&& rhs) :
			m_device(std::move(rhs.m_device)),
			m_semaphore(std::move(rhs.m_semaphore)) {
			rhs.m_semaphore = VK_NULL_HANDLE;
		}

		Semaphore::~Semaphore() {
			if (m_semaphore != VK_NULL_HANDLE) {
				vkDestroySemaphore(m_device, m_semaphore, nullptr);
			}
		}

		Semaphore::operator VkSemaphore() const {
			return m_semaphore;
		}

	}
}