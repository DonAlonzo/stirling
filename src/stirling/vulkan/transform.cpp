#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace stirling {
    namespace vulkan {

        Transform::operator const glm::mat4&() const {
            return m_transform;
        }

        void Transform::rotate(float a, glm::vec3 axis) {
            m_transform = glm::rotate(glm::mat4(), a, axis);
        }

        void Transform::moveTo(const glm::vec3& position) {
            m_transform[0][3] = position.x;
            m_transform[1][3] = position.y;
            m_transform[2][3] = position.z;
        }

        void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
            m_transform = glm::lookAt(glm::vec3(m_transform[0][3], m_transform[1][3], m_transform[2][3]), target, up);
        }

    }
}