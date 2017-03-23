#include "window.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace stirling {

	Window::Window(int width, int height) :
		m_window          (initWindow(width, height)),
		m_instance        (initInstance()),
		m_physical_device (choosePhysicalDevice(m_instance.getPhysicalDevices())) {
	}

	GLFWwindow* Window::initWindow(int width, int height) const {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		return glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
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
		return physical_device.hasAllQueueFamilies();
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

	Window::~Window() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool Window::isRunning() const {
		return !glfwWindowShouldClose(m_window);
	}

	void Window::update() {
		glfwPollEvents();
	}

}