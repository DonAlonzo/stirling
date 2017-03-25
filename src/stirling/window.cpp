#include "window.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#include <set>

const std::vector<const char*> g_device_extensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace stirling {

	Window::Window(int width, int height) :
		m_window          (initWindow(width, height)),
		m_instance        (initInstance()),
		m_surface         (initSurface()),
		m_physical_device (choosePhysicalDevice(m_instance.getPhysicalDevices())),
		m_device          (initDevice()),
		m_swapchain       (initSwapchain()) {
	}

	GLFWwindow* Window::initWindow(int width, int height) const {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		return glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
	}

	vulkan::Surface Window::initSurface() const {
		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface.");
		}
		return vulkan::Surface(surface);
	}

	vulkan::Instance Window::initInstance() const {
		return vulkan::Instance(getRequiredExtensions());
	}
	
	vulkan::PhysicalDevice Window::choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const {
		for (const auto& physical_device : physical_devices) {
			if (isPhysicalDeviceSuitable(physical_device)) {
				return physical_device;
			}
		}
		throw std::runtime_error("Failed to find a suitable GPU.");
	}

	bool Window::isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const {
		auto indices = physical_device.findQueueFamilies(m_surface);
		if (!indices.isComplete()) return false;

		std::set<std::string> required_extensions(g_device_extensions.begin(), g_device_extensions.end());
		for (const auto& available_extension : physical_device.getExtensions()) {
			required_extensions.erase(available_extension.extensionName);
		}
		if (!required_extensions.empty()) return false;

		auto formats = physical_device.getSurfaceFormats(m_surface);
		auto present_modes = physical_device.getSurfacePresentModes(m_surface);
		return !formats.empty() && !present_modes.empty();
	}

	vulkan::Device Window::initDevice() const {
		return m_physical_device.createDevice(m_surface, g_device_extensions);
	}

	vulkan::Swapchain Window::initSwapchain() const {
		return vulkan::Swapchain(m_device, m_surface);
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