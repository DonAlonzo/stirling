#include "camera.h"
#include "input_handler.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace stirling {

    Camera::Camera(float field_of_view, float aspect_ratio, float near_plane, float far_plane) :
        m_field_of_view (field_of_view),
        m_aspect_ratio  (aspect_ratio), 
        m_near_plane    (near_plane),
        m_far_plane     (far_plane) {

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

    void Camera::update(float delta_seconds) {
        if (InputHandler::getInstance()[Action::MOVE_FORWARD]) {
            m_transform.translate(m_transform.forward() * 0.0025f);
        } else if (InputHandler::getInstance()[Action::MOVE_BACKWARD]) {
            m_transform.translate(m_transform.backward() * 0.0025f);
        }

        if (InputHandler::getInstance()[Action::STRAFE_LEFT]) {
            m_transform.translate(m_transform.left() * 0.0025f);
        } else if (InputHandler::getInstance()[Action::STRAFE_RIGHT]) {
            m_transform.translate(m_transform.right() * 0.0025f);
        }

        if (InputHandler::getInstance()[Action::JUMP]) {
            m_transform.translate(m_transform.up() * 0.0025f);
        } else if (InputHandler::getInstance()[Action::CROUCH]) {
            m_transform.translate(m_transform.down() * 0.0025f);
        }

    }

}