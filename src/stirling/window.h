#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// GLFW
#include <GLFW/glfw3.h>

// Stirling
#include "stirling.h"

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
        GLFWwindow*                        m_window;
        vulkan::Instance                   m_instance;
        vulkan::Surface                    m_surface;
        vulkan::PhysicalDevice             m_physical_device;
        vulkan::Device                     m_device;
        vulkan::Swapchain                  m_swapchain;
        vulkan::DepthImage                 m_depth_image;
        vulkan::RenderPass                 m_render_pass;
        vulkan::Pipeline                   m_pipeline;
        std::vector<vulkan::Framebuffer>   m_framebuffers;
        vulkan::CommandPool                m_command_pool;
        vulkan::Model                      m_model;
        vulkan::UniformBuffer              m_uniform_buffer;
        vulkan::DescriptorPool             m_descriptor_pool;
        VkDescriptorSet                    m_descriptor_set;
        std::vector<VkCommandBuffer>       m_command_buffers;
        vulkan::Semaphore                  m_image_available_semaphore;
        vulkan::Semaphore                  m_render_finished_semaphore;

        Camera                             m_camera;
        glm::mat4                          m_projection_matrix;

        GLFWwindow*                        initWindow(int width, int height);
        vulkan::Surface                    initSurface() const;
        vulkan::PhysicalDevice             choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const;
        bool                               isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const;
        vulkan::DescriptorPool             initDescriptorPool() const;
        VkDescriptorSet                    initDescriptorSet() const;
        std::vector<VkCommandBuffer>       initCommandBuffers() const;

        glm::mat4                          getProjectionMatrix() const;

        void                               recreateSwapchain();
        std::vector<const char*>           getRequiredExtensions() const;
        VkExtent2D                         getSize() const;

        static void                        onResized(GLFWwindow* window, int width, int height);
    };

}