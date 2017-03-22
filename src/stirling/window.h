#pragma once

typedef struct GLFWwindow GLFWwindow;

#include "vulkan/instance.h"

namespace stirling {

class Window {
public:
	Window(int width, int height);
			
	~Window();

	Window(const Window& rhs) = delete;
	Window& operator=(const Window& rhs) = delete;

	Window(Window&& rhs) = default;
	Window& operator=(Window&& rhs) = default;

	bool isRunning();
	void update();

private:
	GLFWwindow* createWindow(int width, int height);
	VulkanInstance createInstance();

	std::vector<const char*> getRequiredExtensions();

	GLFWwindow* m_window;
	VulkanInstance m_instance;
};

}