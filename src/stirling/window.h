#pragma once

#include "vulkan/device.h"
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