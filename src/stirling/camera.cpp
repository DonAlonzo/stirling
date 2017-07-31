#include "camera.h"
#include "input_handler.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <string>

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

	void Camera::rotate(float angle, const glm::vec3& axis) {
		m_rotation *= glm::angleAxis(angle, axis);
	}

	void Camera::moveTo(const glm::vec3& position) {
		m_position = position;
	}

	void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
		m_rotation = glm::lookAt(target, m_position, up);
	}

    void Camera::update(float delta_seconds) {
        float meters_per_second = 1.5f;

        glm::vec3 direction;
        if (InputHandler::getInstance()[Action::MOVE_FORWARD])  direction += m_transform.forward();
        if (InputHandler::getInstance()[Action::MOVE_BACKWARD]) direction += m_transform.backward();
        if (InputHandler::getInstance()[Action::STRAFE_LEFT])   direction += m_transform.left();
        if (InputHandler::getInstance()[Action::STRAFE_RIGHT])  direction += m_transform.right();
        if (InputHandler::getInstance()[Action::JUMP])          direction += glm::vec3(0.0f, 0.0f, -1.0f);
        if (InputHandler::getInstance()[Action::CROUCH])        direction += glm::vec3(0.0f, 0.0f, 1.0f);
        
        if (glm::length(direction) > 0.0f) {
			m_position += glm::normalize(direction) * meters_per_second * delta_seconds;
        }

		m_transform = glm::translate(glm::toMat4(m_rotation), m_position);
    }

}