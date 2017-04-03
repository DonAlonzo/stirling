#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}

namespace stirling {
	namespace vulkan {
		class Semaphore {
		public:
			Semaphore(const Device& device);
			~Semaphore();
			Semaphore(Semaphore&&);
			Semaphore(const Semaphore&) = delete;
			Semaphore& operator=(Semaphore&&);
			Semaphore& operator=(const Semaphore&) = delete;

			operator VkSemaphore() const;
			
		private:
			const Device* m_device;
			VkSemaphore   m_semaphore;

			VkSemaphore   initSemaphore() const;
		};
	}
}