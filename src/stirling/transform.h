#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>

namespace stirling {
    namespace vulkan {

        struct Transform {
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale{ 1.0f };

            Transform();
            Transform(const glm::mat4& transform);
            Transform(glm::mat4&& transform);
            Transform(glm::mat4* transform);

            operator const glm::mat4&();
			Transform operator*(const Transform&) const;

            void lookAt(const glm::vec3& target, const glm::vec3& center, const glm::vec3& up = glm::vec3(0.0f, 0.0f, 1.0f));

            glm::vec3 left() const;
            glm::vec3 right() const;
            glm::vec3 down() const;
            glm::vec3 up() const;
            glm::vec3 forward() const;
            glm::vec3 backward() const;

            void update();

        private:
            std::shared_ptr<glm::mat4> transform;
        };

    }
}