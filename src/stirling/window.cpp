#include "window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace stirling {

Window::Window(int width, int height) :
	m_window   (initWindow(width, height)),
	m_instance (initInstance()) {
}

GLFWwindow* Window::initWindow(int width, int height) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	return glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
}

VulkanInstance Window::initInstance() {
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