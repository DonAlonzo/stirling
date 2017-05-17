#include "stirling/window.h"
#include "stirling/map.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <iostream>

class StirlingDemo {
public:
    StirlingDemo() :
        m_window (1680, 1050, createMap()) {
    }

    stirling::Map createMap() {
        stirling::MaterialPool material_pool;
        std::vector<stirling::EntityCreateInfo> entity_create_info_list;

        auto material = material_pool.allocate();
        material->vertex_shader_file   = "shaders/vert.spv";
        material->fragment_shader_file = "shaders/frag.spv";

        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                stirling::EntityCreateInfo create_info;
                create_info.position     = glm::vec3(x * 2.f, y * 2.f, 0.f);
                create_info.rotation     = glm::vec3(0.f, 0.f, 0.f);
                create_info.scale        = glm::vec3(1.f, 1.f, 1.f);
                create_info.model_file   = "models/chalet.obj";
                create_info.texture_file = "textures/chalet.jpg";
                create_info.material     = material;
                entity_create_info_list.push_back(create_info);
            }
        }

        stirling::EntityCreateInfo create_info = {};
        create_info.position     = glm::vec3(-2.f, -1.f, 0.f);
        create_info.rotation     = glm::vec3(0.f, 0.f, 0.f);
        create_info.scale        = glm::vec3(0.1f, 0.1f, 0.1f);
        create_info.model_file   = "models/agesoflove.obj";
        create_info.texture_file = "textures/agesoflove.jpg";
        create_info.material     = material;
        //entity_create_info_list.push_back(create_info);

        return { std::move(material_pool), entity_create_info_list };
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