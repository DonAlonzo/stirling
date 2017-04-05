#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace stirling {

    Camera::Camera(float field_of_view, float aspect_ratio, float near_plane, float far_plane) :
        m_field_of_view     (field_of_view),
        m_aspect_ratio      (aspect_ratio), 
        m_near_plane        (near_plane),
        m_far_plane         (far_plane) {

        updateProjectionMatrix();
    }

    void Camera::setAspectRatio(float aspect_ratio) {
        m_aspect_ratio = aspect_ratio;
        updateProjectionMatrix();
    }

    void Camera::setFieldOfView(float field_of_view) {
        m_field_of_view = field_of_view;
        updateProjectionMatrix();
    }

    void Camera::setNearPlane(float near_plane) {
        m_near_plane = near_plane;
        updateProjectionMatrix();
    }

    void Camera::setFarPlane(float far_plane) {
        m_far_plane = far_plane;
        updateProjectionMatrix();
    }

    void Camera::updateProjectionMatrix() {
        m_projection_matrix = glm::perspective(m_field_of_view, m_aspect_ratio, m_near_plane, m_far_plane);
        m_projection_matrix[1][1] *= -1;
    }

    const glm::mat4& Camera::getProjectionMatrix() const {
        return m_projection_matrix;
    }

}