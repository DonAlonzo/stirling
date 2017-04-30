#include "semaphore.h"
#include "device.h"

namespace stirling {
    namespace vulkan {

        Semaphore::Semaphore(const Device& device) :
            m_semaphore (initSemaphore(device)) {
        }

        Deleter<VkSemaphore> Semaphore::initSemaphore(const Device& device) const {
            VkSemaphoreCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkSemaphore semaphore;
            if (vkCreateSemaphore(device, &create_info, nullptr, &semaphore) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphore.");
            }
            return Deleter<VkSemaphore>(semaphore, device, vkDestroySemaphore);
        }

        Semaphore::operator VkSemaphore() const {
            return m_semaphore;
        }

    }
}