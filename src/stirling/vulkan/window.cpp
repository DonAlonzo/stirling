#include "window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace stirling {
	namespace vulkan {
		Window::Window(int width, int height) {
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			m_window = glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
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
}