#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// GLFW
#include <GLFW/glfw3.h>

// Stirling
#include "vulkan/command_pool.h"
#include "vulkan/device.h"
#include "vulkan/instance.h"
#include "vulkan/pipeline.h"
#include "vulkan/physical_device.h"
#include "vulkan/render_pass.h"
#include "vulkan/semaphore.h"
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
		Window& operator=(Window&&) = delete;
		Window& operator=(const Window&) = delete;

		bool isRunning() const;
		void update();

	private:
		GLFWwindow*                      m_window;
		vulkan::Instance                 m_instance                  = initInstance();
		vulkan::Surface                  m_surface                   = initSurface();
		vulkan::PhysicalDevice           m_physical_device           = choosePhysicalDevice(m_instance.getPhysicalDevices());
		vulkan::Device                   m_device                    = initDevice();
		vulkan::Swapchain                m_swapchain                 = initSwapchain();
		vulkan::RenderPass               m_render_pass               = initRenderPass();
		vulkan::Pipeline                 m_pipeline                  = initPipeline();
		std::vector<vulkan::Framebuffer> m_framebuffers              = initFramebuffers();
		vulkan::CommandPool              m_command_pool              = initCommandPool();
		std::vector<VkCommandBuffer>     m_command_buffers           = initCommandBuffers();
		vulkan::Semaphore                m_image_available_semaphore   {m_device};
		vulkan::Semaphore                m_render_finished_semaphore   {m_device};
		
		GLFWwindow*                      initWindow(int width, int height);
		vulkan::Instance                 initInstance() const;
		vulkan::Surface                  initSurface() const;
		vulkan::PhysicalDevice           choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const;
		bool                             isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const;
		vulkan::Device                   initDevice() const;
		vulkan::Swapchain                initSwapchain() const;
		vulkan::RenderPass               initRenderPass() const;
		vulkan::Pipeline                 initPipeline() const;
		std::vector<vulkan::Framebuffer> initFramebuffers() const;
		vulkan::CommandPool              initCommandPool() const;
		std::vector<VkCommandBuffer>     initCommandBuffers() const;

		void                             recreateSwapchain();
		std::vector<const char*>         getRequiredExtensions() const;
		VkExtent2D                       getSize() const;

		static void                      onResized(GLFWwindow* window, int width, int height);
	};

}