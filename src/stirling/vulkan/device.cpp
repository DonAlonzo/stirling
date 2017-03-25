#include "device.h"
#include "physical_device.h"

#include <vector>

namespace stirling {
	namespace vulkan {

		Device::Device(const PhysicalDevice& physical_device, VkDevice device, QueueFamilyIndices indices) :
			m_physical_device (physical_device),
			m_device          (device),
			m_graphics_queue  (initQueue(indices.graphics_family_index)),
			m_present_queue   (initQueue(indices.present_family_index)) {
		}

		Queue Device::initQueue(int queue_family_index) {
			VkQueue queue;
			vkGetDeviceQueue(m_device, queue_family_index, 0, &queue);
			return Queue(*this, queue);
		}

		Device::~Device() {
			vkDestroyDevice(m_device, nullptr);
		}

		Device::operator VkDevice() const {
			return m_device;
		}

		const PhysicalDevice& Device::getPhysicalDevice() const {
			return m_physical_device;
		}

		VkSwapchainKHR Device::createSwapchain(VkSwapchainCreateInfoKHR create_info) const {
			VkSwapchainKHR swapchain;
			if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, &swapchain) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create swap chain.");
			}
			return swapchain;
		}

		std::vector<VkImage> Device::getSwapchainImages(VkSwapchainKHR swapchain, uint32_t image_count) const {
			vkGetSwapchainImagesKHR(m_device, swapchain, &image_count, nullptr);
			std::vector<VkImage> images{image_count};
			vkGetSwapchainImagesKHR(m_device, swapchain, &image_count, images.data());
			return images;

		}

	}
}