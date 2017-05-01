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
        Window(int width, int height);
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
        vulkan::Surface                   m_surface;
        vulkan::PhysicalDevice            m_physical_device;
        vulkan::Device                    m_device;
        vulkan::Swapchain                 m_swapchain;
        vulkan::DepthImage                m_depth_image;
        vulkan::RenderPass                m_render_pass;
        std::vector<vulkan::Framebuffer>  m_framebuffers;
        vulkan::CommandPool               m_command_pool;
        vulkan::Semaphore                 m_image_available_semaphore;
        vulkan::Semaphore                 m_render_finished_semaphore;
        vulkan::Buffer                    m_static_uniform_buffer;
        vulkan::Buffer                    m_dynamic_uniform_buffer;
        size_t                            m_dynamic_alignment;

        std::unique_ptr<Camera>           m_camera;
        World                             m_world;

        VkDescriptorSetLayout             m_descriptor_set_layout;
        vulkan::DescriptorPool            m_descriptor_pool;
        
        vulkan::Pipeline                  m_pipeline;

        std::unique_ptr<ModelComponent>   m_house_model_component;
        std::unique_ptr<PhysicsComponent> m_house_physics_component;
        std::unique_ptr<Entity>           m_house_entity_1;
        std::unique_ptr<Entity>           m_house_entity_2;

        std::vector<VkCommandBuffer>      m_command_buffers;

        // Initializer methods

        GLFWwindow*                       initWindow(int width, int height);
        vulkan::Surface                   initSurface() const;
        vulkan::PhysicalDevice            choosePhysicalDevice(const std::vector<vulkan::PhysicalDevice>& physical_devices) const;
        bool                              isPhysicalDeviceSuitable(const vulkan::PhysicalDevice& physical_device) const;
        vulkan::Buffer                    initStaticUniformBuffer(const vulkan::Device& device);
        vulkan::Buffer                    initDynamicUniformBuffer(const vulkan::Device& device, int max_number_of_objects);

        VkDescriptorSetLayout             initDescriptorSetLayout() const;
        vulkan::DescriptorPool            initDescriptorPool() const;
        std::vector<VkCommandBuffer>      initCommandBuffers() const;
        ModelComponent*                   createHouseModelComponent() const;
        PhysicsComponent*                 createHousePhysicsComponent() const;
        Entity*                           createHouseEntity(ModelComponent* model_component, PhysicsComponent* physics_component) const;

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