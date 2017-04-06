#include "window.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>
#include <set>

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

    Window::Window(int width, int height) :
        m_window                    (initWindow(width, height)),
        m_instance                  (vulkan::Instance(getRequiredExtensions())),
        m_surface                   (initSurface()),
        m_physical_device           (choosePhysicalDevice(m_instance.getPhysicalDevices())),
        m_device                    (m_physical_device.createDevice(m_surface, g_device_extensions)),
        m_swapchain                 (vulkan::Swapchain(m_device, m_surface, getSize())),
        m_depth_image               (vulkan::DepthImage(m_device, m_swapchain.getExtent())),
        m_render_pass               (vulkan::RenderPass(m_device, m_swapchain.getImageFormat(), m_depth_image.getImageFormat())),
        m_pipeline                  (vulkan::Pipeline(m_device, m_render_pass, m_swapchain.getExtent())),
        m_framebuffers              (m_swapchain.createFramebuffers(m_render_pass, m_depth_image.getImageView())),
        m_command_pool              (m_device.getGraphicsQueue().createCommandPool()),
        m_model                     (vulkan::Model::loadFromFile(m_device, "models/chalet.obj", "textures/chalet.jpg")),
        m_uniform_buffer            (vulkan::UniformBuffer(m_device)),
        m_descriptor_pool           (initDescriptorPool()),
        m_descriptor_set            (initDescriptorSet()),
        m_command_buffers           (initCommandBuffers()),
        m_image_available_semaphore (vulkan::Semaphore{m_device}),
        m_render_finished_semaphore (vulkan::Semaphore{m_device}),
        m_camera                    (Camera(glm::radians(60.0f), m_swapchain.getExtent().width / (float)m_swapchain.getExtent().height, 0.1f, 10.0f)) {

        m_camera.moveTo(glm::vec3(2.0f, 2.0f, 2.0f));
        m_camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    GLFWwindow* Window::initWindow(int width, int height) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        auto window = glfwCreateWindow(width, height, "Stirling Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, WindowListener::onResized);
        glfwSetKeyCallback(window, WindowListener::onKeyInput);
        glfwSetCursorPosCallback(window, WindowListener::onMouseMovementInput);
        glfwSetMouseButtonCallback(window, WindowListener::onMouseButtonInput);
        glfwSetScrollCallback(window, WindowListener::onMouseScrollInput);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        return window;
    }

    vulkan::Surface Window::initSurface() const {
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface.");
        }
        return vulkan::Surface(m_instance, surface);
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

    vulkan::DescriptorPool Window::initDescriptorPool() const {
        std::vector<VkDescriptorPoolSize> pool_sizes{2};

        pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pool_sizes[0].descriptorCount = 1;

        pool_sizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[1].descriptorCount = 1;

        return vulkan::DescriptorPool(m_device, pool_sizes, 1);
    }

    VkDescriptorSet Window::initDescriptorSet() const {
        auto descriptor_set = m_descriptor_pool.allocateDescriptorSet(m_pipeline.getDescriptorSetLayout());

        VkDescriptorBufferInfo buffer_info = {};
        buffer_info.buffer = m_uniform_buffer;
        buffer_info.offset = 0;
        buffer_info.range  = sizeof(vulkan::UniformBufferObject);

        VkDescriptorImageInfo image_info = {};
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_info.imageView   = m_model.getTexture().getImageView();
        image_info.sampler     = m_model.getTexture().getSampler();

        std::array<VkWriteDescriptorSet, 2> write_descriptor_sets = {};

        write_descriptor_sets[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_descriptor_sets[0].dstSet           = descriptor_set;
        write_descriptor_sets[0].dstBinding       = 0;
        write_descriptor_sets[0].dstArrayElement  = 0;
        write_descriptor_sets[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write_descriptor_sets[0].descriptorCount  = 1;
        write_descriptor_sets[0].pBufferInfo      = &buffer_info;

        write_descriptor_sets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_descriptor_sets[1].dstSet          = descriptor_set;
        write_descriptor_sets[1].dstBinding      = 1;
        write_descriptor_sets[1].dstArrayElement = 0;
        write_descriptor_sets[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write_descriptor_sets[1].descriptorCount = 1;
        write_descriptor_sets[1].pImageInfo      = &image_info;

        vkUpdateDescriptorSets(m_device, write_descriptor_sets.size(), write_descriptor_sets.data(), 0, nullptr);

        return descriptor_set;
    }

    std::vector<VkCommandBuffer> Window::initCommandBuffers() const {
        auto command_buffers = m_command_pool.allocateCommandBuffers(m_framebuffers.size());
        for (int i = 0; i < command_buffers.size(); ++i) {
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
                vkCmdBindPipeline(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

                VkBuffer vertex_buffers[] = { m_model.getVertexBuffer() };
                VkDeviceSize offsets[] = { 0 };
                vkCmdBindVertexBuffers(command_buffers[i], 0, 1, vertex_buffers, offsets);

                vkCmdBindIndexBuffer(command_buffers[i], m_model.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

                vkCmdBindDescriptorSets(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.getLayout(), 0, 1, &m_descriptor_set, 0, nullptr);

                vkCmdDrawIndexed(command_buffers[i], m_model.getIndexBuffer().size(), 1, 0, 0, 0);
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

    void Window::recreateSwapchain() {
        vkDeviceWaitIdle(m_device);

        m_swapchain.reset(getSize());
        m_depth_image  = vulkan::DepthImage(m_device, m_swapchain.getExtent());
        m_render_pass  = vulkan::RenderPass(m_device, m_swapchain.getImageFormat(), m_depth_image.getImageFormat());
        m_pipeline     = vulkan::Pipeline(m_device, m_render_pass, m_swapchain.getExtent());
        m_framebuffers = m_swapchain.createFramebuffers(m_render_pass, m_depth_image.getImageView());

        vkFreeCommandBuffers(m_device, m_command_pool, m_command_buffers.size(), m_command_buffers.data());
        m_command_buffers = initCommandBuffers();

        m_camera.setAspectRatio(m_swapchain.getExtent().width / (float)m_swapchain.getExtent().height);
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
        { // Rotate model
            static auto last_time = std::chrono::high_resolution_clock::now();
            auto current_time = std::chrono::high_resolution_clock::now();
            float delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count() / 1000.0f;
            last_time = current_time;

            m_model.rotate(delta_time * 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
        }

        { // Update model uniform
            vulkan::UniformBufferObject ubo = {};
            ubo.model      = m_model;
            ubo.view       = m_camera;
            ubo.projection = m_camera.getProjectionMatrix();
            m_uniform_buffer.update(ubo);
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

    void Window::onResized(int width, int height) {
        if (width == 0 || height == 0) return;

        recreateSwapchain();
    }

    void Window::onKeyInput(int key, int scancode, int action, int mods) {
        std::cout << std::to_string(key) << std::endl;
        if (action == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_W:
                m_camera.translate(m_camera.forward() * 0.25f);
                break;
            case GLFW_KEY_A:
                m_camera.translate(m_camera.left() * 0.25f);
                break;
            case GLFW_KEY_S:
                m_camera.translate(m_camera.backward() * 0.25f);
                break;
            case GLFW_KEY_D:
                m_camera.translate(m_camera.right() * 0.25f);
                break;
            case GLFW_KEY_SPACE:
                m_camera.translate(m_camera.up() * 0.25f);
                break;
            case GLFW_KEY_LEFT_CONTROL:
                m_camera.translate(m_camera.down() * 0.25f);
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
                break;
            case GLFW_KEY_F11:
                if (glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED)) {
                    glfwRestoreWindow(m_window);
                } else {
                    glfwMaximizeWindow(m_window);
                }
                break;
            }
        }
    }

    float last_x, last_y;
    void Window::onMouseMovementInput(double x, double y) {
        double delta_x = x - last_x;
        double delta_y = y - last_y;
        m_camera.rotate(delta_x * 0.001f, m_camera.up());
        m_camera.rotate(delta_y * 0.001f, m_camera.right());
        last_x = x;
        last_y = y;
    }

    void Window::onMouseButtonInput(int button, int action, int mods) {
    }

    void Window::onMouseScrollInput(double xoffset, double yoffset) {
    }

}