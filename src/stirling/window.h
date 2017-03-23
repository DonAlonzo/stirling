#pragma once

#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#include <vector>

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
		GLFWwindow*          m_window;
		VulkanInstance       m_instance;
		VulkanPhysicalDevice m_physical_device;

		GLFWwindow*          initWindow(int width, int height) const;
		VulkanInstance       initInstance() const;
		VulkanPhysicalDevice choosePhysicalDevice(const std::vector<VulkanPhysicalDevice>& physical_devices) const;
		bool                 isPhysicalDeviceSuitable(const VulkanPhysicalDevice& physical_device) const;

		std::vector<const char*> getRequiredExtensions() const;
	};

}