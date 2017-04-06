#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace stirling {
    namespace vulkan {
        class Transform {
        public:
            operator const glm::mat4&();

            void translate(const glm::vec3& translation);
            void moveTo(const glm::vec3& position);
            void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 0.0f, 1.0f));
            void rotate(float angle, const glm::vec3& axis);

            glm::vec3 left() const;
            glm::vec3 right() const;
            glm::vec3 down() const;
            glm::vec3 up() const;
            glm::vec3 forward() const;
            glm::vec3 backward() const;

            glm::vec3 position() const;

        private:
            glm::vec3 m_position;
            glm::quat m_rotation;
            glm::mat4 m_transform;

            bool m_update_transform = true;
        };
    }
}