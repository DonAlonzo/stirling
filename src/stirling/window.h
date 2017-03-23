#pragma once

#include "vulkan/instance.h"

typedef struct GLFWwindow GLFWwindow;

namespace stirling {

class Window {
public:
	Window(int width, int height);
			
	~Window();

	Window(const Window& rhs) = delete;
	Window& operator=(const Window& rhs) = delete;

	Window(Window&& rhs) = default;
	Window& operator=(Window&& rhs) = default;

	bool isRunning() const;
	void update();

private:
	GLFWwindow*    m_window;
	VulkanInstance m_instance;

	GLFWwindow*    initWindow(int width, int height);
	VulkanInstance initInstance();

	std::vector<const char*> getRequiredExtensions();
};

}