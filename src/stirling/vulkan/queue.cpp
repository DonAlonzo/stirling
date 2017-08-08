#include "queue.h"
#include "command_pool.h"
#include "device.h"

namespace stirling {
	namespace vulkan {

		Queue::Queue(const Device& device, int family_index) :
			device       (device),
			family_index (family_index),
			queue        (initQueue()) {
		}

		VkQueue Queue::initQueue() const {
			VkQueue queue;
			vkGetDeviceQueue(device, family_index, 0, &queue);
			return queue;
		}

		Queue::operator VkQueue() const {
			return queue;
		}

		CommandPool Queue::createCommandPool() const {
			return CommandPool(device, family_index);
		}

	}
}