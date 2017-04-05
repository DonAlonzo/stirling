#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace stirling {
    namespace vulkan {

        Transform::operator const glm::mat4&() const {
            return m_transform;
        }

        void Transform::translate(const glm::vec3& translation) {
            m_transform *= glm::translate(glm::mat4(), translation);
        }

        void Transform::rotate(float a, glm::vec3 axis) {
            m_transform *= glm::rotate(glm::mat4(), a, axis);
        }

        void Transform::moveTo(const glm::vec3& position) {
            m_transform[0][3] = position.x;
            m_transform[1][3] = position.y;
            m_transform[2][3] = position.z;
        }

        void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
            m_transform = glm::lookAt(glm::vec3(m_transform[0][3], m_transform[1][3], m_transform[2][3]), target, up);
        }

        glm::vec4 Transform::left() const {
            return glm::vec4(m_transform[0][0], m_transform[1][0], m_transform[2][0], 0.0f);
        }

        glm::vec4 Transform::right() const {
            return -left();
        }

        glm::vec4 Transform::down() const {
            return glm::vec4(m_transform[0][1], m_transform[1][1], m_transform[2][1], 0.0f);;
        }

        glm::vec4 Transform::up() const {
            return -down();
        }

        glm::vec4 Transform::forward() const {
            return glm::vec4(m_transform[0][2], m_transform[1][2], m_transform[2][2], 0.0f);
        }

        glm::vec4 Transform::backward() const {
            return -forward();
        }

    }
}