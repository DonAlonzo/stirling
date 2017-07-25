#include "stirling/window.h"
#include "stirling/map.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <iostream>

class StirlingDemo {
public:
    StirlingDemo() :
        m_window (1680, 1050, createMapBlueprint()) {
    }

    stirling::MapBlueprint createMapBlueprint() {
        stirling::MapBlueprint map_blueprint;

        auto material = map_blueprint.createMaterial();
        material->addShader(VK_SHADER_STAGE_VERTEX_BIT, "shaders/vert.spv", "main");
        material->addShader(VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/frag.spv", "main");

        for (int x = -1; x <= 1; ++x) {
			for (int y = 0; y <= 0; ++y) {
				stirling::EntityCreateInfo create_info = {};
                create_info.position     = glm::vec3(x * 2.f, y * 2.f, 0.f);
                create_info.rotation     = glm::vec3(0.f, 0.f, 0.f);
                create_info.scale        = glm::vec3(1.f, 1.f, 1.f);
                create_info.model_file   = "models/chalet.obj";
                create_info.texture_file = "textures/chalet.jpg";
                create_info.material     = material;
                map_blueprint.addEntity(create_info);
            }
        }

		for (int x = -1; x <= 1; ++x) {
			for (int y = 0; y <= 0; ++y) {
				stirling::EntityCreateInfo create_info = {};
				create_info.position     = glm::vec3(x * -20.f, y * -10.f, 0.f);
				create_info.rotation     = glm::vec3(0.f, 0.f, glm::radians(180.f));
				create_info.scale        = glm::vec3(0.1f, 0.1f, 0.1f);
				create_info.model_file   = "models/gladiators.obj";
				create_info.texture_file = "textures/gladiators.jpg";
				create_info.material     = material;
				map_blueprint.addEntity(create_info);
			}
		}

        return map_blueprint;
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
		StirlingDemo{}.run();
        return 0;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}