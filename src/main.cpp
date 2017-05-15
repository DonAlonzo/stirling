#include "stirling/window.h"
#include "stirling/map.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class StirlingDemo {
public:
    StirlingDemo() :
        m_window (1680, 1050, createMap()) {
    }

    stirling::Map createMap() {
        std::vector<stirling::EntityCreateInfo> entity_create_info_list;
        
        stirling::EntityCreateInfo create_info;
        create_info.position             = glm::vec3(0.f, 1.f, 0.f);
        create_info.rotation             = glm::vec3(0.f, 0.f, 0.f);
        create_info.scale                = glm::vec3(1.f, 1.f, 1.f);
        create_info.model_file           = "models/chalet.obj";
        create_info.texture_file         = "textures/chalet.jpg";
        create_info.vertex_shader_file   = "shaders/vert.spv";
        create_info.fragment_shader_file = "shaders/frag.spv";
        entity_create_info_list.push_back(create_info);
        
        create_info = {};
        create_info.position             = glm::vec3(1.f, 0.f, 1.f);
        create_info.rotation             = glm::vec3(0.f, 0.f, 0.f);
        create_info.scale                = glm::vec3(1.f, 1.f, 1.f);
        create_info.model_file           = "models/chalet.obj";
        create_info.texture_file         = "textures/chalet.jpg";
        create_info.vertex_shader_file   = "shaders/vert.spv";
        create_info.fragment_shader_file = "shaders/frag.spv";
        entity_create_info_list.push_back(create_info);
        
        return entity_create_info_list;
    }

    void run() {
        while (m_window.isRunning()) {
            m_window.update();
            m_window.render();
        }
    }

private:
    stirling::Window m_window;
};

int main() {
    try {
        StirlingDemo().run();
        return 0;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}