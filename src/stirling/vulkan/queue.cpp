#include "queue.h"

#include "device.h"

namespace stirling {
	namespace vulkan {

		Queue::Queue(const Device& device, VkQueue queue) :
			m_device (device),
			m_queue  (queue) {
		}

	}
}