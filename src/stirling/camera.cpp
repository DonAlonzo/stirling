#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace stirling {

    Camera::Camera() {
    }

    void Camera::moveTo(const glm::vec3& position) {
        m_view_matrix[0][3] = position.x;
        m_view_matrix[1][3] = position.y;
        m_view_matrix[2][3] = position.z;
    }

    void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
        m_view_matrix = glm::lookAt(glm::vec3(m_view_matrix[0][3], m_view_matrix[1][3], m_view_matrix[2][3]), target, up);
    }

    Camera::operator const glm::mat4&() const {
        return m_view_matrix;
    }

}