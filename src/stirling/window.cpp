#include "window.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace stirling {

	Window::Window(int width, int height) :
		m_window          (initWindow(width, height)),
		m_instance        (initInstance()),
		m_surface         (initSurface()),
		m_physical_device (choosePhysicalDevice(m_instance.getPhysicalDevices())),
		m_device          (initDevice()) {
	}

	GLFWwindow* Window::initWindow(int width, int height) const {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		return glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
	}

	VkSurfaceKHR Window::initSurface() const {
		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface.");
		}
		return surface;
	}

	VulkanInstance Window::initInstance() const {
		return VulkanInstance(getRequiredExtensions());
	}
	
	VulkanPhysicalDevice Window::choosePhysicalDevice(const std::vector<VulkanPhysicalDevice>& physical_devices) const {
		for (const auto& physical_device : physical_devices) {
			if (isPhysicalDeviceSuitable(physical_device)) {
				return physical_device;
			}
		}
		throw std::runtime_error("Failed to find a suitable GPU.");
	}

	bool Window::isPhysicalDeviceSuitable(const VulkanPhysicalDevice& physical_device) const {
		return physical_device.findQueueFamilies(m_surface).isComplete();
	}

	VulkanDevice Window::initDevice() const {
		return VulkanDevice(m_physical_device, m_physical_device.findQueueFamilies(m_surface));
	}

	Window::~Window() {
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	std::vector<const char*> Window::getRequiredExtensions() const {
		std::vector<const char*> extensions;

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (unsigned int i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		return extensions;
	}

	bool Window::isRunning() const {
		return !glfwWindowShouldClose(m_window);
	}

	void Window::update() {
		glfwPollEvents();
	}

}