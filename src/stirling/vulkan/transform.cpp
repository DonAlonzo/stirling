#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

namespace stirling {
    namespace vulkan {

        Transform::Transform() {
            m_transform = new glm::mat4();
        }

        Transform::Transform(glm::mat4* transform) {
            m_transform = transform;
        }

        Transform::operator const glm::mat4&() {
            return transform();
        }

        void Transform::translate(const glm::vec3& translation) {
            m_position += translation;
            update();
        }

        void Transform::moveTo(const glm::vec3& position) {
            m_position = position;
            update();
        }

        void Transform::rotate(float angle, const glm::vec3& axis) {
            m_rotation *= glm::angleAxis(angle, axis);
            update();
        }

        void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
            m_rotation = glm::lookAt(target, m_position, up);
            update();
        }

        void Transform::setScale(const glm::vec3& scale) {
            m_scale = scale;
            update();
        }

        glm::vec3 Transform::left() const {
            return glm::vec3((*m_transform)[0][0], (*m_transform)[1][0], (*m_transform)[2][0]) ;
        }

        glm::vec3 Transform::right() const {
            return -left();
        }

        glm::vec3 Transform::down() const {
            return glm::vec3((*m_transform)[0][1], (*m_transform)[1][1], (*m_transform)[2][1]) ;
        }

        glm::vec3 Transform::up() const {
            return -down();
        }

        glm::vec3 Transform::forward() const {
            return glm::vec3((*m_transform)[0][2], (*m_transform)[1][2], (*m_transform)[2][2]);
        }

        glm::vec3 Transform::backward() const {
            return -forward();
        }

        const glm::vec3& Transform::position() const {
            return m_position;
        }

        const glm::quat& Transform::rotation() const {
            return m_rotation;
        }

        const glm::vec3& Transform::scale() const {
            return m_scale;
        }

        const glm::mat4& Transform::transform() {
            return *m_transform;
        }

        void Transform::update() {
            *m_transform = glm::translate(glm::toMat4(m_rotation) * glm::scale(glm::mat4(), m_scale), m_position);
        }

    }
}