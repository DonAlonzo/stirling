#include "window.h"
#include "vulkan/initializers.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <chrono>
#include <iostream>
#include <set>

#ifdef NDEBUG
#define ENABLE_VALIDATION_LAYERS false
#else
#define ENABLE_VALIDATION_LAYERS true
#endif

const std::vector<const char*> g_device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace stirling {
    class WindowListener {
        friend Window;

        static void onResized(GLFWwindow* glfw_window, int width, int height) {
            reinterpret_cast<stirling::Window*>(glfwGetWindowUserPointer(glfw_window))->onResized(width, height);
        }

        static void onKeyInput(GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
            reinterpret_cast<stirling::Window*>(glfwGetWindowUserPointer(glfw_window))->onKeyInput(key, scancode, action, mods);
        }

        static void onMouseMovementInput(GLFWwindow* glfw_window, double xpos, double ypos) {
            reinterpret_cast<stirling::Window*>(glfwGetWindowUserPointer(glfw_window))->onMouseMovementInput(xpos, ypos);
        }

        static void onMouseButtonInput(GLFWwindow* glfw_window, int button, int action, int mods) {
            reinterpret_cast<stirling::Window*>(glfwGetWindowUserPointer(glfw_window))->onMouseButtonInput(button, action, mods);
        }

        static void onMouseScrollInput(GLFWwindow* glfw_window, double xoffset, double yoffset) {
            reinterpret_cast<stirling::Window*>(glfwGetWindowUserPointer(glfw_window))->onMouseScrollInput(xoffset, yoffset);
        }
    };
}

namespace stirling {

    Window::Window(int width, int height, MapBlueprint map_blueprint) :
        m_window                    (initWindow(width, height)),
        m_instance                  (getRequiredExtensions()),
        m_surface                   (initSurface()),
        m_physical_device           (choosePhysicalDevice(m_instance.getPhysicalDevices())),
        m_device                    (m_physical_device.createDevice(m_surface, g_device_extensions)),
        m_swapchain                 (m_device, m_surface, getSize()),
        m_depth_image               (m_device, m_swapchain.getExtent()),
        m_render_pass               (m_device, m_swapchain.getImageFormat(), m_depth_image.image_format),
        m_framebuffers              (m_swapchain.createFramebuffers(m_render_pass, m_depth_image.image_view)),
        m_image_available_semaphore (m_device.createSemaphore()),
        m_render_finished_semaphore (m_device.createSemaphore()),
        m_camera                    (glm::radians(60.0f), m_swapchain.getExtent().width / (float)m_swapchain.getExtent().height, 0.01f, 100.0f),
        m_map                       (map_blueprint.instantiate(m_device, m_render_pass, m_swapchain.getExtent())),
        m_command_pool              (m_device.getGraphicsQueue().createCommandPool()),
        m_command_buffers           (initCommandBuffers()) {
        
        // Add map entities to world
        for (auto& entity : m_map.entities) {
            m_world.addEntity(&entity);
        }

        addControls();

        // Add camera
        m_camera.moveTo(glm::vec3(2.f, -2.f, -2.f));
        m_camera.lookAt(glm::vec3(0.f, 0.f, 0.f));
        m_world.addEntity(&m_camera);
    }

    GLFWwindow* Window::initWindow(int width, int height) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        auto window = glfwCreateWindow(width, height, "Stirling Engine", nullptr/*glfwGetPrimaryMonitor()*/, nullptr);
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
		glfwSetWindowPos(window, 0, 0);

        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, WindowListener::onResized);
        glfwSetKeyCallback(window, WindowListener::onKeyInput);
        glfwSetCursorPosCallback(window, WindowListener::onMouseMovementInput);
        glfwSetMouseButtonCallback(window, WindowListener::onMouseButtonInput);
        glfwSetScrollCallback(window, WindowListener::onMouseScrollInput);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        return window;
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

    vulkan::Deleter<VkSurfaceKHR> Window::initSurface() const {
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface.");
        }
        return vulkan::Deleter<VkSurfaceKHR>(surface, m_instance, vkDestroySurfaceKHR);
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
        for (const auto& available_extension : physical_device.extensions) {
            required_extensions.erase(available_extension.extensionName);
        }
        if (!required_extensions.empty()) return false;

        auto formats = physical_device.getSurfaceFormats(m_surface);
        auto present_modes = physical_device.getSurfacePresentModes(m_surface);
        return !formats.empty() && !present_modes.empty();
    }

    std::vector<VkCommandBuffer> Window::initCommandBuffers() const {
        auto command_buffers = m_command_pool.allocateCommandBuffers(m_framebuffers.size());
        for (int i = 0; i < command_buffers.size(); ++i) {
			std::cout << "Creating command buffer (" << std::to_string(i + 1) << "/" << std::to_string(command_buffers.size()) << ")" << std::endl;

            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            vkBeginCommandBuffer(command_buffers[i], &begin_info);

            std::array<VkClearValue, 2> clear_values = {};
            clear_values[0].color        = { 0.0f, 0.0f, 0.0f, 1.0f };
            clear_values[1].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo render_pass_info = {};
            render_pass_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            render_pass_info.renderPass        = m_render_pass;
            render_pass_info.framebuffer       = m_framebuffers[i];
            render_pass_info.renderArea.offset = { 0, 0 };
            render_pass_info.renderArea.extent = m_swapchain.getExtent();
            render_pass_info.clearValueCount   = clear_values.size();
            render_pass_info.pClearValues      = clear_values.data();

            vkCmdBeginRenderPass(command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
            for (auto& render_instruction : m_map.render_instructions) {
                vkCmdBindPipeline(
                    command_buffers[i],
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    render_instruction.pipeline
                );

                vkCmdBindVertexBuffers(
                    command_buffers[i], 0, 1,
                    render_instruction.vertex_buffers.data(),
                    render_instruction.vertex_buffer_offsets.data()
                );

                vkCmdBindIndexBuffer(
                    command_buffers[i],
                    render_instruction.index_buffer,
                    0, VK_INDEX_TYPE_UINT32
                );

                vkCmdBindDescriptorSets(
                    command_buffers[i],
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    render_instruction.pipeline_layout, 0,
                    render_instruction.descriptor_sets.size(), render_instruction.descriptor_sets.data(),
                    render_instruction.dynamic_offsets.size(), render_instruction.dynamic_offsets.data()
                );

                vkCmdDrawIndexed(
                    command_buffers[i],
                    render_instruction.index_count,
                    1, 0, 0, 0
                );
            }
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

    void Window::addControls() {
        InputHandler::getInstance().addCommand(Action::EXIT, [this]() {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        });

        InputHandler::getInstance().addCommand(Action::FULL_SCREEN, [this]() {
            if (glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED)) {
                glfwRestoreWindow(m_window);
            } else {
                glfwMaximizeWindow(m_window);
            }
        });
    }

    void Window::recreateSwapchain() {
        throw std::runtime_error("Window::recreateSwapchain hasn't been implemented yet.");
        vkDeviceWaitIdle(m_device);

        m_swapchain    = vulkan::Swapchain{m_device, m_surface, getSize(), m_swapchain};
        m_depth_image  = vulkan::DepthImage{m_device, m_swapchain.getExtent()};
        m_render_pass  = vulkan::RenderPass{m_device, m_swapchain.getImageFormat(), m_depth_image.image_format};
        m_framebuffers = m_swapchain.createFramebuffers(m_render_pass, m_depth_image.image_view);

		/*
		pipeline.recreate(m_render_pass, m_swapchain.getExtent());

        m_pipeline = vulkan::Pipeline(m_device, { m_descriptor_set_layout }, m_render_pass, m_swapchain.getExtent(), "shaders/vert.spv", "shaders/frag.spv");
		*/

        vkFreeCommandBuffers(m_device, m_command_pool, m_command_buffers.size(), m_command_buffers.data());
        m_command_buffers = initCommandBuffers();

        m_camera.setAspectRatio(m_swapchain.getExtent().width / (float)m_swapchain.getExtent().height);
    }

    VkExtent2D Window::getSize() const {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return VkExtent2D{(uint32_t)width, (uint32_t)height};
    }

    bool Window::isRunning() const {
        glfwPollEvents();
        return !glfwWindowShouldClose(m_window);
    }

    void Window::update() {
        // Game loop
        static auto last_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        m_world.update(std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_time).count() / 1E6f);
        last_time = current_time;
    }

    void Window::render() {
		// Calculate FPS
		glfwSetWindowTitle(m_window, ("Stirling Engine - FPS: " + std::to_string(calculateFPS())).c_str());

        { // Update static uniform buffer
            m_map.static_uniform_buffer_object.view       = m_camera.transform();
            m_map.static_uniform_buffer_object.projection = m_camera.getProjectionMatrix();
            m_map.static_uniform_buffer_mapping.memcpy(&m_map.static_uniform_buffer_object, m_map.static_uniform_buffer.size);
        }

        { // Update dynamic uniform buffer
            m_map.dynamic_uniform_buffer_mapping.memcpy(m_map.dynamic_uniform_buffer_object.model, m_map.dynamic_uniform_buffer.size);

            // Flush dynamic uniform buffer memory
            VkMappedMemoryRange mapped_memory_range = {};
            mapped_memory_range.sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            mapped_memory_range.memory = m_map.dynamic_uniform_buffer.memory;
            mapped_memory_range.size   = m_map.dynamic_uniform_buffer.size;
            vkFlushMappedMemoryRanges(m_device, 1, &mapped_memory_range);
        }

        uint32_t image_index;
        switch (vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<uint64_t>::max(), m_image_available_semaphore, VK_NULL_HANDLE, &image_index)) {
        case VK_SUBOPTIMAL_KHR:
        case VK_SUCCESS:
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            recreateSwapchain();
            return;
        default:
            throw std::runtime_error("Failed to acquire swapchain image.");
        }

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

        switch (vkQueuePresentKHR(m_device.getPresentQueue(), &present_info)) {
        case VK_SUCCESS:
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
        case VK_SUBOPTIMAL_KHR:
            recreateSwapchain();
            return;
        default:
            throw std::runtime_error("Failed to present swapchain image.");
        }
    }

	size_t Window::calculateFPS() const {
		static constexpr std::chrono::milliseconds period(200);
		static std::chrono::time_point<std::chrono::steady_clock> last_tick;
		static size_t frames = 0;
		static size_t fps;

		++frames;

		const auto now = std::chrono::high_resolution_clock::now();
		const auto difference = now - last_tick;
		if (difference > period) {
			fps = 1e6 * frames / std::chrono::duration_cast<std::chrono::microseconds>(difference).count();
			frames = 0;
			last_tick = now;
		}

		return fps;
	}

    void Window::onResized(int width, int height) {
        if (width == 0 || height == 0) return;

        recreateSwapchain();
    }

    void Window::onKeyInput(int key, int scancode, int action, int mods) {
        InputHandler::getInstance().onKeyInput(key, scancode, action, mods);
    }

    double last_x, last_y;
    void Window::onMouseMovementInput(double x, double y) {
        double delta_x = last_x - x;
        double delta_y = last_y - y;
        m_camera.rotate(delta_y * 0.001f, m_camera.transform().right());
        m_camera.rotate(-delta_x * 0.001f, glm::vec3(0.0f, 0.0f, 1.0f));
        last_x = x;
        last_y = y;
    }

    void Window::onMouseButtonInput(int button, int action, int mods) {
    }

    void Window::onMouseScrollInput(double xoffset, double yoffset) {
    }

}