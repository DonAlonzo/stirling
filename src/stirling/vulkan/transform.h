#pragma once

#include <glm/glm.hpp>

namespace stirling {
    namespace vulkan {
        class Transform {
        public:
            operator const glm::mat4&() const;

            void translate(const glm::vec3& translation);
            void moveTo(const glm::vec3& position);
            void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 0.0f, 1.0f));
            void rotate(float a, glm::vec3 axis);

            glm::vec4 left() const;
            glm::vec4 right() const;
            glm::vec4 down() const;
            glm::vec4 up() const;
            glm::vec4 forward() const;
            glm::vec4 backward() const;

        private:
            glm::mat4 m_transform;
        };
    }
}