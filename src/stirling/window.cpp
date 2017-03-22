#include "window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

namespace stirling {

Window::Window(int width, int height) :
	m_window (createWindow(width, height)),
	m_instance (createInstance()) {
}

GLFWwindow* Window::createWindow(int width, int height) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	return glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
}

VulkanInstance Window::createInstance() {
	return VulkanInstance(getRequiredExtensions());
}

std::vector<const char*> Window::getRequiredExtensions() {
	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (ENABLE_VALIDATION_LAYERS) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Window::isRunning() {
	return !glfwWindowShouldClose(m_window);
}

void Window::update() {
	glfwPollEvents();
}

}