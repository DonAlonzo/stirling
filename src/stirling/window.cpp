#include "window.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#include <set>

const std::vector<const char*> g_device_extensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace stirling {

	Window::Window(int width, int height) :
		m_window (initWindow(width, height)) {
	}

	GLFWwindow* Window::initWindow(int width, int height) {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		
		auto window = glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetWindowSizeCallback(window, Window::onResized);
		
		return window;
	}

	vulkan::Surface Window::initSurface() const {
		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface.");
		}
		return vulkan::Surface(m_instance, surface);
	}

	vulkan::Instance Window::initInstance() const {
		return vulkan::Instance(getRequiredExtensions());
	}
	
	vulkan::PhysicalDevice Window::choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const {
		for (const auto& physical_device : physical_devices) {
			if (isPhysicalDeviceSuitable(physical_device)) {
				return physical_device;
			}
		}
		throw std::runtime_error("Failed to find a suitable GPU.");
	}

	bool Window::isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const {
		auto indices = physical_device.findQueueFamilies(m_surface);
		if (!indices.isComplete()) return false;

		std::set<std::string> required_extensions(g_device_extensions.begin(), g_device_extensions.end());
		for (const auto& available_extension : physical_device.getExtensions()) {
			required_extensions.erase(available_extension.extensionName);
		}
		if (!required_extensions.empty()) return false;

		auto formats = physical_device.getSurfaceFormats(m_surface);
		auto present_modes = physical_device.getSurfacePresentModes(m_surface);
		return !formats.empty() && !present_modes.empty();
	}

	vulkan::Device Window::initDevice() const {
		return m_physical_device.createDevice(m_surface, g_device_extensions);
	}

	vulkan::Swapchain Window::initSwapchain() const {
		return vulkan::Swapchain(m_device, m_surface, getSize());
	}

	vulkan::RenderPass Window::initRenderPass() const {
		return vulkan::RenderPass(m_device, m_swapchain.getImageFormat());
	}

	vulkan::Pipeline Window::initPipeline() const {
		return vulkan::Pipeline(m_device, m_render_pass, m_swapchain.getExtent());
	}

	std::vector<vulkan::Framebuffer> Window::initFramebuffers() const {
		return m_swapchain.createFramebuffers(m_render_pass);
	}
	
	vulkan::CommandPool Window::initCommandPool() const {
		return m_device.getGraphicsQueue().createCommandPool();
	}
	
	std::vector<VkCommandBuffer> Window::initCommandBuffers() const {
		auto command_buffers = m_command_pool.allocateCommandBuffers(m_framebuffers.size());
		for (int i = 0; i < command_buffers.size(); ++i) {
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			vkBeginCommandBuffer(command_buffers[i], &begin_info);

			VkClearValue clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };

			VkRenderPassBeginInfo render_pass_info = {};
			render_pass_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			render_pass_info.renderPass        = m_render_pass;
			render_pass_info.framebuffer       = m_framebuffers[i];
			render_pass_info.renderArea.offset = { 0, 0 };
			render_pass_info.renderArea.extent = m_swapchain.getExtent();
			render_pass_info.clearValueCount   = 1;
			render_pass_info.pClearValues      = &clear_color;

			vkCmdBeginRenderPass(command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
				vkCmdBindPipeline(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
				vkCmdDraw(command_buffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(command_buffers[i]);

			if (vkEndCommandBuffer(command_buffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to record command buffer.");
			}
		}
		return command_buffers;
	}

	Window::~Window() {
		vkDeviceWaitIdle(m_device);

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::onResized(GLFWwindow* window, int width, int height) {
		if (width == 0 || height == 0) return;

		reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->recreateSwapChain();
	}

	void Window::recreateSwapChain() {
		vkDeviceWaitIdle(m_device);

		m_swapchain.reset(getSize());
		m_render_pass.reset(m_swapchain.getImageFormat());
		m_pipeline.reset(m_render_pass, m_swapchain.getExtent());
		m_framebuffers    = initFramebuffers();

		vkFreeCommandBuffers(m_device, m_command_pool, m_command_buffers.size(), m_command_buffers.data());
		m_command_buffers = initCommandBuffers();
	}

	std::vector<const char*> Window::getRequiredExtensions() const {
		std::vector<const char*> extensions;

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (unsigned int i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		return extensions;
	}

	VkExtent2D Window::getSize() const {
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);
		return VkExtent2D { (uint32_t) width, (uint32_t) height };
	}


	bool Window::isRunning() const {
		glfwPollEvents();
		return !glfwWindowShouldClose(m_window);
	}

	void Window::update() {
		uint32_t image_index;
		vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<uint64_t>::max(), m_image_available_semaphore, VK_NULL_HANDLE, &image_index);

		VkSemaphore wait_semaphores[] = {
			m_image_available_semaphore
		};

		VkPipelineStageFlags wait_stages[] = {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		};

		VkSemaphore signal_semaphores[] = {
			m_render_finished_semaphore
		};

		VkSubmitInfo submit_info = {};
		submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount   = 1;
		submit_info.pWaitSemaphores      = wait_semaphores;
		submit_info.pWaitDstStageMask    = wait_stages;
		submit_info.commandBufferCount   = 1;
		submit_info.pCommandBuffers      = &m_command_buffers[image_index];
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores    = signal_semaphores;

		if (vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw std::runtime_error("Failed to submit draw command buffer.");
		}

		VkSwapchainKHR swap_chains[] = {
			m_swapchain
		};

		VkPresentInfoKHR present_info = {};
		present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores    = signal_semaphores;
		present_info.swapchainCount     = 1;
		present_info.pSwapchains        = swap_chains;
		present_info.pImageIndices      = &image_index;
		present_info.pResults           = nullptr;

		vkQueuePresentKHR(m_device.getPresentQueue(), &present_info);
	}

}