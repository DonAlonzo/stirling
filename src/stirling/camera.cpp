#include "camera.h"
#include "input_handler.h"
#include "util/math.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <string>

namespace stirling {

    Camera::Camera(float field_of_view, float aspect_ratio, float near_plane, float far_plane) :
        field_of_view (field_of_view),
        aspect_ratio  (aspect_ratio), 
        near_plane    (near_plane),
        far_plane     (far_plane) {

        updateProjectionMatrix();
    }

    void Camera::setAspectRatio(float aspect_ratio) {
        this->aspect_ratio = aspect_ratio;
        updateProjectionMatrix();
    }

    void Camera::setFieldOfView(float field_of_view) {
        this->field_of_view = field_of_view;
        updateProjectionMatrix();
    }

    void Camera::setNearPlane(float near_plane) {
        this->near_plane = near_plane;
        updateProjectionMatrix();
    }

    void Camera::setFarPlane(float far_plane) {
        this->far_plane = far_plane;
        updateProjectionMatrix();
    }

    void Camera::updateProjectionMatrix() {
        projection_matrix = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
        projection_matrix[1][1] *= -1;
    }

	void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
        const auto direction = glm::normalize(target - position);
        pitch = asin(direction.y);
        yaw = atan2(direction.x, direction.z);
	}

    void Camera::update(float delta_seconds) {
        float meters_per_second = 1.5f;

        glm::vec3 direction;
        if (InputHandler::instance[Action::MOVE_FORWARD])  direction += transform.forward();
        if (InputHandler::instance[Action::MOVE_BACKWARD]) direction += transform.backward();
        if (InputHandler::instance[Action::STRAFE_LEFT])   direction += transform.left();
        if (InputHandler::instance[Action::STRAFE_RIGHT])  direction += transform.right();
        if (InputHandler::instance[Action::JUMP])          direction += glm::vec3(0.f, -1.f, 0.f);
        if (InputHandler::instance[Action::CROUCH])        direction += glm::vec3(0.f, 1.f, 0.f);
        
        if (glm::length(direction) > 0.f) {
			position += glm::normalize(direction) * meters_per_second * delta_seconds;
        }

        pitch = util::clamp(pitch, -glm::radians(85.f), glm::radians(85.f));
        yaw = fmod(yaw, glm::radians(360.f));
        
        rotation  = glm::angleAxis(yaw, glm::vec3{ 0.f, 1.f, 0.f });
        rotation *= glm::angleAxis(pitch, glm::vec3{ 1.f, 0.f, 0.f } * rotation);
        rotation *= glm::angleAxis(roll, glm::vec3{ 0.f, 0.f, 1.f } * rotation);
        
		transform = glm::translate(glm::toMat4(rotation), position);
    }

}