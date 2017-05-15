#pragma once

// Vulkan
#include "vulkan/vulkan.h"

// GLFW
#include <GLFW/glfw3.h>

// Stirling
#include "stirling.h"

// std
#include <memory>
#include <vector>

namespace stirling {
    
    class WindowListener;
    class Window {
        friend WindowListener;

    public:
        Window(int width, int height, Map map);
        ~Window();
        Window(Window&&) = default;
        Window(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        Window& operator=(const Window&) = delete;

        bool isRunning() const;
        void render();
        void update();

    private:
        GLFWwindow*                       m_window;
        vulkan::Instance                  m_instance;
        vulkan::Deleter<VkSurfaceKHR>     m_surface;
        vulkan::PhysicalDevice            m_physical_device;
        vulkan::Device                    m_device;
        vulkan::Swapchain                 m_swapchain;
        vulkan::DepthImage                m_depth_image;
        vulkan::RenderPass                m_render_pass;
        std::vector<VkFramebuffer>        m_framebuffers;
        vulkan::CommandPool               m_command_pool;
        vulkan::Semaphore                 m_image_available_semaphore;
        vulkan::Semaphore                 m_render_finished_semaphore;

        Camera                            m_camera;
        World                             m_world;
        MapInstance                       m_map_instance;

        std::vector<VkCommandBuffer>      m_command_buffers;

        // Initializer methods

        GLFWwindow*                       initWindow(int width, int height);
        vulkan::Deleter<VkSurfaceKHR>     initSurface() const;
        vulkan::PhysicalDevice            choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const;
        bool                              isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const;
        std::vector<VkCommandBuffer>      initCommandBuffers() const;

        void                              addControls();
        void                              recreateSwapchain();
        std::vector<const char*>          getRequiredExtensions() const;
        VkExtent2D                        getSize() const;

        void                              onResized(int width, int height);
        void                              onKeyInput(int key, int scancode, int action, int mods);
        void                              onMouseMovementInput(double x, double y);
        void                              onMouseButtonInput(int button, int action, int mods);
        void                              onMouseScrollInput(double xoffset, double yoffset);
    };

}