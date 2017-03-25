#include "queue.h"
#include "command_pool.h"
#include "device.h"

namespace stirling {
	namespace vulkan {

		Queue::Queue(const Device& device, int family_index) :
			m_device       (device),
			m_family_index (family_index),
			m_queue        (initQueue()) {
		}

		VkQueue Queue::initQueue() const {
			VkQueue queue;
			vkGetDeviceQueue(m_device, m_family_index, 0, &queue);
			return queue;
		}

		CommandPool Queue::createCommandPool() const {
			return CommandPool(m_device, m_family_index);
		}

	}
}