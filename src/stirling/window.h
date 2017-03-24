#pragma once

// Stirling
#include "vulkan/device.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

// std
#include <vector>

// GLFW
typedef struct GLFWwindow GLFWwindow;

namespace stirling {

	class Window {
	public:
		Window(int width, int height);
		~Window();
		Window(Window&&) = default;
		Window(const Window&) = delete;
		Window& operator=(Window&&) = default;
		Window& operator=(const Window&) = delete;

		bool isRunning() const;
		void update();

	private:
		GLFWwindow*          m_window;
		VulkanInstance       m_instance;
		VkSurfaceKHR         m_surface;
		VulkanPhysicalDevice m_physical_device;
		VulkanDevice         m_device;

		GLFWwindow*          initWindow(int width, int height) const;
		
		VulkanInstance       initInstance() const;
		
		VkSurfaceKHR         initSurface() const;

		VulkanPhysicalDevice choosePhysicalDevice(const std::vector<VulkanPhysicalDevice>& physical_devices) const;
		bool                 isPhysicalDeviceSuitable(const VulkanPhysicalDevice& physical_device) const;
		
		VulkanDevice         initDevice() const;

		std::vector<const char*> getRequiredExtensions() const;
	};

}