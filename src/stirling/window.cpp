#include "window.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <set>

const std::vector<const char*> g_device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace stirling {

    Window::Window(int width, int height) :
        m_window                    (initWindow(width, height)),
        m_instance                  (initInstance()),
        m_surface                   (initSurface()),
        m_physical_device           (choosePhysicalDevice(m_instance.getPhysicalDevices())),
        m_device                    (initDevice()),
        m_swapchain                 (initSwapchain()),
        m_depth_image               (initDepthImage()),
        m_render_pass               (initRenderPass()),
        m_pipeline                  (initPipeline()),
        m_framebuffers              (initFramebuffers()),
        m_command_pool              (initCommandPool()),
        m_model                     (loadModel()),
        m_uniform_buffer            (initUniformBuffer()),
        m_descriptor_pool           (initDescriptorPool()),
        m_descriptor_set            (initDescriptorSet()),
        m_command_buffers           (initCommandBuffers()),
        m_image_available_semaphore (vulkan::Semaphore{m_device}),
        m_render_finished_semaphore (vulkan::Semaphore{m_device}) {

        m_projection_matrix = getProjectionMatrix();

        m_camera.moveTo(glm::vec3(2.0f, 2.0f, 2.0f));
        m_camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
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
        return vulkan::Swapchain(m_device, m_surface, getSize(), VK_NULL_HANDLE);
    }

    vulkan::DepthImage Window::initDepthImage() const {
        return vulkan::DepthImage(m_device, m_swapchain.getExtent());
    }

    vulkan::RenderPass Window::initRenderPass() const {
        return vulkan::RenderPass(m_device, m_swapchain.getImageFormat(), m_depth_image.getImageFormat());
    }

    vulkan::Pipeline Window::initPipeline() const {
        return vulkan::Pipeline(m_device, m_render_pass, m_swapchain.getExtent());
    }

    std::vector<vulkan::Framebuffer> Window::initFramebuffers() const {
        return m_swapchain.createFramebuffers(m_render_pass, m_depth_image.getImageView());
    }
    
    vulkan::CommandPool Window::initCommandPool() const {
        return m_device.getGraphicsQueue().createCommandPool();
    }

    vulkan::Model Window::loadModel() const {
        return vulkan::Model::loadFromFile(m_device, "models/chalet.obj", "textures/chalet.jpg");
    }

    vulkan::UniformBuffer Window::initUniformBuffer() const {
        return vulkan::UniformBuffer(m_device);
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

    void Window::onResized(GLFWwindow* window, int width, int height) {
        if (width == 0 || height == 0) return;

        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->recreateSwapchain();
    }

    void Window::recreateSwapchain() {
        vkDeviceWaitIdle(m_device);

        m_swapchain         = vulkan::Swapchain(m_device, m_surface, getSize(), m_swapchain);
        m_projection_matrix = getProjectionMatrix();
        m_depth_image       = initDepthImage();
        m_render_pass       = initRenderPass();
        m_pipeline          = initPipeline();
        m_framebuffers      = initFramebuffers();

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
        // Update uniform buffer
        static auto start_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() / 1000.0f;

        vulkan::UniformBufferObject ubo = {};
        ubo.model             = glm::rotate(glm::mat4(), 0.1f * time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view              = m_camera;
        ubo.projection        = m_projection_matrix;
        ubo.projection[1][1] *= -1;

        m_uniform_buffer.update(ubo);
        // TODO: Move this into a separate camera class

        uint32_t image_index;
        switch (vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<uint64_t>::max(), m_image_available_semaphore, VK_NULL_HANDLE, &image_index)) {
        case VK_ERROR_OUT_OF_DATE_KHR:
            recreateSwapchain();
            return;
        case VK_SUBOPTIMAL_KHR:
        case VK_SUCCESS:
            break;
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
        case VK_ERROR_OUT_OF_DATE_KHR:
        case VK_SUBOPTIMAL_KHR:
            recreateSwapchain();
            return;
        case VK_SUCCESS:
            break;
        default:
            throw std::runtime_error("Failed to present swapchain image.");
        }
    }

    glm::mat4 Window::getProjectionMatrix() const {
        return glm::perspective(glm::radians(60.0f), m_swapchain.getExtent().width / (float)m_swapchain.getExtent().height, 0.1f, 10.0f);
    }

}