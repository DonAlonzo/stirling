#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

namespace stirling {
    namespace vulkan {

        Transform::operator const glm::mat4&() {
            if (m_update_transform) {
                m_transform = glm::toMat4(m_rotation);
                m_transform = glm::translate(m_transform, m_position);
                m_update_transform = false;
            }
            return m_transform;
        }

        void Transform::translate(const glm::vec3& translation) {
            m_position += translation;
            m_update_transform = true;
        }

        void Transform::moveTo(const glm::vec3& position) {
            m_position = position;
            m_update_transform = true;
        }

        void Transform::rotate(float angle, const glm::vec3& axis) {
            m_rotation *= glm::angleAxis(angle, axis);
            m_update_transform = true;
        }

        void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
            m_rotation = glm::lookAt(target, m_position, up);
            m_update_transform = true;
        }

        glm::vec3 Transform::left() const {
            return glm::vec3(m_transform[0][0], m_transform[1][0], m_transform[2][0]) ;
        }

        glm::vec3 Transform::right() const {
            return -left();
        }

        glm::vec3 Transform::down() const {
            return glm::vec3(m_transform[0][1], m_transform[1][1], m_transform[2][1]) ;
        }

        glm::vec3 Transform::up() const {
            return -down();
        }

        glm::vec3 Transform::forward() const {
            return glm::vec3(m_transform[0][2], m_transform[1][2], m_transform[2][2]);
        }

        glm::vec3 Transform::backward() const {
            return -forward();
        }

        const glm::vec3& Transform::position() const {
            return m_position;
        }

    }
}