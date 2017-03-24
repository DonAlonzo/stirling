#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {

	class VulkanQueue {
	public:
		VulkanQueue(VkQueue);

	private:
		VkQueue m_queue;
	};

}