#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// GLFW
#include <GLFW/glfw3.h>

// Stirling
#include "vulkan/device.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"
#include "vulkan/surface.h"
#include "vulkan/swapchain.h"

// std
#include <vector>

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
		GLFWwindow*            m_window;
		vulkan::Instance       m_instance;
		vulkan::Surface        m_surface;
		vulkan::PhysicalDevice m_physical_device;
		vulkan::Device         m_device;
		vulkan::Swapchain      m_swapchain;

		GLFWwindow*            initWindow(int width, int height) const;
		vulkan::Instance       initInstance() const;
		vulkan::Surface        initSurface() const;
		vulkan::PhysicalDevice choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const;
		bool                   isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const;
		vulkan::Device         initDevice() const;
		vulkan::Swapchain      initSwapchain() const;

		std::vector<const char*> getRequiredExtensions() const;
	};

}