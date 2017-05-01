#include "window.h"
#include "vulkan/initializers.h"
#include "vulkan/instance.h"
#include "vulkan/physical_device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>
#include <set>

#ifdef NDEBUG
#define ENABLE_VALIDATION_LAYERS false
#else
#define ENABLE_VALIDATION_LAYERS true
#endif

void* alignedAlloc(size_t size, size_t alignment) {
    void *data = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__)
    data = _aligned_malloc(size, alignment);
#else 
    int res = posix_memalign(&data, alignment, size);
    if (res != 0) {
        data = nullptr;
    }
#endif
    return data;
}

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

    struct StaticUniformBufferObject {
        glm::mat4 projection;
        glm::mat4 view;
    } m_static_ubo;

    struct DynamicUniformBufferObject {
        glm::mat4* model = nullptr;
    } m_dynamic_ubo;

    Window::Window(int width, int height) :
        m_window                    (initWindow(width, height)),
        m_instance                  (vulkan::Instance(getRequiredExtensions())),
        m_surface                   (initSurface()),
        m_physical_device           (choosePhysicalDevice(m_instance.getPhysicalDevices())),
        m_device                    (m_physical_device.createDevice(m_surface, g_device_extensions)),
        m_swapchain                 (vulkan::Swapchain(m_device, m_surface, getSize())),
        m_depth_image               (vulkan::DepthImage(m_device, m_swapchain.getExtent())),
        m_static_uniform_buffer     (initStaticUniformBuffer(m_device)),
        m_dynamic_uniform_buffer    (initDynamicUniformBuffer(m_device, 256)),

        m_house_model_component     (createHouseModelComponent()),
        m_house_physics_component   (createHousePhysicsComponent()),
        m_house_entity_1            (createHouseEntity(m_house_model_component.get(), m_house_physics_component.get())),
        m_house_entity_2            (createHouseEntity(m_house_model_component.get(), m_house_physics_component.get())),
        m_render_pass               (vulkan::RenderPass(m_device, m_swapchain.getImageFormat(), m_depth_image.getImageFormat())),
        m_descriptor_set_layout     (initDescriptorSetLayout()),
        m_descriptor_pool           (initDescriptorPool()),
        m_descriptor_set            (initDescriptorSet()),
        m_pipeline                  (vulkan::Pipeline(m_device, { m_descriptor_set_layout }, m_render_pass, m_swapchain.getExtent())),
        m_framebuffers              (m_swapchain.createFramebuffers(m_render_pass, m_depth_image.getImageView())),
        m_command_pool              (m_device.getGraphicsQueue().createCommandPool()),
        m_command_buffers           (initCommandBuffers()),

        m_image_available_semaphore (m_device.createSemaphore()),
        m_render_finished_semaphore (m_device.createSemaphore()) {

        addControls();

        m_camera = std::unique_ptr<Camera>(new Camera(glm::radians(60.0f), m_swapchain.getExtent().width / (float)m_swapchain.getExtent().height, 0.01f, 10.0f));

        glfwMaximizeWindow(m_window);

        m_world.addEntity(m_camera.get());
        m_world.addEntity(m_house_entity_1.get());
        m_world.addEntity(m_house_entity_2.get());

        m_camera->transform().moveTo(glm::vec3(2.0f, -2.0f, -2.0f));
        m_camera->transform().lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        m_house_entity_2->transform().moveTo(glm::vec3(-2.0f, -2.0f, 0.0f));
        m_house_entity_2->transform().setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    GLFWwindow* Window::initWindow(int width, int height) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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

    vulkan::Buffer Window::initStaticUniformBuffer(const vulkan::Device& device) {
        auto buffer = device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sizeof(StaticUniformBufferObject));
        buffer.map();
        return buffer;
    }

    vulkan::Buffer Window::initDynamicUniformBuffer(const vulkan::Device& device, int max_number_of_objects) {
        size_t ubo_alignment = m_physical_device.getProperties().limits.minUniformBufferOffsetAlignment;

        m_dynamic_alignment = (sizeof(glm::mat4) / ubo_alignment) * ubo_alignment + ((sizeof(glm::mat4) % ubo_alignment) > 0 ? ubo_alignment : 0);

        size_t buffer_size = max_number_of_objects * m_dynamic_alignment;
        m_dynamic_ubo.model = (glm::mat4*)alignedAlloc(buffer_size, m_dynamic_alignment);

        auto buffer = device.createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, buffer_size);
        buffer.map();
        return buffer;
    }

    ModelComponent* Window::createHouseModelComponent() const {
        auto model_component = new ModelComponent(vulkan::Model::loadFromFile(m_device, "models/chalet.obj", "textures/chalet.jpg"));

        return model_component;
    }

    PhysicsComponent* Window::createHousePhysicsComponent() const {
        auto physics_component = new PhysicsComponent();

        return physics_component;
    }

    Entity* Window::createHouseEntity(ModelComponent* model_component, PhysicsComponent* physics_component) const {
        auto entity = new Entity();

        entity->addComponent(physics_component);
        entity->addComponent(model_component);

        return entity;
    }

    VkDescriptorSetLayout Window::initDescriptorSetLayout() const {
        std::array<VkDescriptorSetLayoutBinding, 3> bindings = {
            vulkan::initializers::descriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT),
            vulkan::initializers::descriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT),
            vulkan::initializers::descriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        VkDescriptorSetLayoutCreateInfo create_info = {};
        create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        create_info.bindingCount = bindings.size();
        create_info.pBindings    = bindings.data();

        VkDescriptorSetLayout descriptor_set_layout;
        if (vkCreateDescriptorSetLayout(m_device, &create_info, nullptr, &descriptor_set_layout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set layout.");
        }
        return descriptor_set_layout;
    }

    vulkan::DescriptorPool Window::initDescriptorPool() const {
        std::vector<VkDescriptorPoolSize> pool_sizes{3};

        pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pool_sizes[0].descriptorCount = 1;

        pool_sizes[1].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        pool_sizes[1].descriptorCount = 1;

        pool_sizes[2].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[2].descriptorCount = 1;

        return vulkan::DescriptorPool(m_device, pool_sizes, 1);
    }

    VkDescriptorSet Window::initDescriptorSet() {
        auto descriptor_set = m_descriptor_pool.allocateDescriptorSet(m_descriptor_set_layout);

        VkDescriptorImageInfo image_info = {};
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_info.imageView   = m_house_model_component->model().getTexture().getImageView();
        image_info.sampler     = m_house_model_component->model().getTexture().getSampler();

        std::array<VkWriteDescriptorSet, 3> write_descriptor_sets = {
            vulkan::initializers::writeDescriptorSet(descriptor_set, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &m_static_uniform_buffer.m_descriptor),
            vulkan::initializers::writeDescriptorSet(descriptor_set, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &m_dynamic_uniform_buffer.m_descriptor),
            vulkan::initializers::writeDescriptorSet(descriptor_set, 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &image_info)
        };

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

                VkBuffer vertex_buffers[] = { m_house_model_component->model().getVertexBuffer() };
                VkDeviceSize offsets[] = { 0 };
                vkCmdBindVertexBuffers(command_buffers[i], 0, 1, vertex_buffers, offsets);
                vkCmdBindIndexBuffer(command_buffers[i], m_house_model_component->model().getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

                for (uint32_t j = 0; j < 2; ++j) {
                    uint32_t dynamic_offset = j * static_cast<uint32_t>(m_dynamic_alignment);

                    vkCmdBindDescriptorSets(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.getLayout(), 0, 1, &m_descriptor_set, 1, &dynamic_offset);

                    vkCmdDrawIndexed(command_buffers[i], m_house_model_component->model().getIndexBuffer().size(), 1, 0, 0, 0);
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
        vkDeviceWaitIdle(m_device);

        m_swapchain.reset(getSize());
        m_depth_image  = vulkan::DepthImage(m_device, m_swapchain.getExtent());
        m_render_pass  = vulkan::RenderPass(m_device, m_swapchain.getImageFormat(), m_depth_image.getImageFormat());
        m_pipeline     = vulkan::Pipeline(m_device, { m_descriptor_set_layout }, m_render_pass, m_swapchain.getExtent());
        m_framebuffers = m_swapchain.createFramebuffers(m_render_pass, m_depth_image.getImageView());

        vkFreeCommandBuffers(m_device, m_command_pool, m_command_buffers.size(), m_command_buffers.data());
        m_command_buffers = initCommandBuffers();

        m_camera->setAspectRatio(m_swapchain.getExtent().width / (float)m_swapchain.getExtent().height);
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
        // Game loop
        static auto last_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        m_world.update(std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count() / 1000.0f);
        last_time = current_time;
    }

    void Window::render() {
        { // Update static uniform buffer
            m_static_ubo.view       = m_camera->transform();
            m_static_ubo.projection = m_camera->getProjectionMatrix();
            m_static_uniform_buffer.memcpy(&m_static_ubo);
        }

        { // Update dynamic uniform buffer
            {
                *((glm::mat4*)(((uint64_t)m_dynamic_ubo.model + (0 * m_dynamic_alignment)))) = m_house_entity_1->transform().transform();
                *((glm::mat4*)(((uint64_t)m_dynamic_ubo.model + (1 * m_dynamic_alignment)))) = m_house_entity_2->transform().transform();
            }

            m_dynamic_uniform_buffer.memcpy(m_dynamic_ubo.model);

            // Flush dynamic uniform buffer memory
            VkMappedMemoryRange mapped_memory_range = {};
            mapped_memory_range.sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            mapped_memory_range.memory = m_dynamic_uniform_buffer.m_memory;
            mapped_memory_range.size   = m_dynamic_uniform_buffer.m_size;
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

    void Window::onResized(int width, int height) {
        if (width == 0 || height == 0) return;

        recreateSwapchain();
    }

    void Window::onKeyInput(int key, int scancode, int action, int mods) {
        InputHandler::getInstance().onKeyInput(key, scancode, action, mods);
    }

    float last_x, last_y;
    void Window::onMouseMovementInput(double x, double y) {
        double delta_x = last_x - x;
        double delta_y = last_y - y;
        m_camera->transform().rotate(delta_y * 0.001f, m_camera->transform().right());
        m_camera->transform().rotate(-delta_x * 0.001f, glm::vec3(0.0f, 0.0f, 1.0f));
        last_x = x;
        last_y = y;
    }

    void Window::onMouseButtonInput(int button, int action, int mods) {
    }

    void Window::onMouseScrollInput(double xoffset, double yoffset) {
    }

}