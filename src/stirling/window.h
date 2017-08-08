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
    
    struct WindowListener;

    struct Window {
        friend WindowListener;

        Window(int width, int height, MapBlueprint map);
        ~Window();
        Window(Window&&) = default;
        Window(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        Window& operator=(const Window&) = delete;

        bool isRunning() const;
        void render();
        void update();

    private:
        GLFWwindow*                   window;
        vulkan::Instance              instance;
        vulkan::Deleter<VkSurfaceKHR> surface;
        vulkan::PhysicalDevice        physical_device;
        vulkan::Device                device;
        vulkan::Swapchain             swapchain;
        vulkan::DepthImage            depth_image;
        vulkan::RenderPass            render_pass;
        std::vector<VkFramebuffer>    framebuffers;
        vulkan::Deleter<VkSemaphore>  image_available_semaphore;
        vulkan::Deleter<VkSemaphore>  render_finished_semaphore;
        Camera                        camera;
        World                         world;
        Map                           map;
        vulkan::CommandPool           command_pool;
        std::vector<VkCommandBuffer>  command_buffers;

        GLFWwindow*                   initWindow(int width, int height);
        vulkan::Deleter<VkSurfaceKHR> initSurface() const;
        vulkan::PhysicalDevice        choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const;
        bool                          isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const;
        std::vector<VkCommandBuffer>  initCommandBuffers() const;
        void                          addControls();
        void                          recreateSwapchain();
        std::vector<const char*>      getRequiredExtensions() const;
        VkExtent2D                    getSize() const;
		size_t                        calculateFPS() const;
        void                          onResized(int width, int height);
        void                          onKeyInput(int key, int scancode, int action, int mods);
        void                          onMouseMovementInput(double x, double y);
        void                          onMouseButtonInput(int button, int action, int mods);
        void                          onMouseScrollInput(double xoffset, double yoffset);
    };

}