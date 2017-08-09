#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

namespace stirling {
    namespace vulkan {

        Transform::Transform() :
			transform (new glm::mat4()) {
		}

        Transform::Transform(glm::mat4* transform) :
            transform (transform, [](glm::mat4*) { /* don't delete when referenceless */ }) {
		}

		Transform::Transform(const glm::mat4& transform) :
			transform (new glm::mat4(transform)) {
		}
        
        Transform::operator const glm::mat4&() {
            return *transform;
        }

		Transform Transform::operator*(const Transform& rhs) const {
			return (*transform) * (*rhs.transform);
		}

        void Transform::lookAt(const glm::vec3& target, const glm::vec3& center, const glm::vec3& up) {
            rotation = glm::lookAt(target, center, up);
        }

        glm::vec3 Transform::left() const {
            return {(*transform)[0][0], (*transform)[1][0], (*transform)[2][0]};
        }

        glm::vec3 Transform::right() const {
            return -left();
        }

        glm::vec3 Transform::down() const {
            return {(*transform)[0][1], (*transform)[1][1], (*transform)[2][1]};
        }

        glm::vec3 Transform::up() const {
            return -down();
        }

        glm::vec3 Transform::forward() const {
            return {(*transform)[0][2], (*transform)[1][2], (*transform)[2][2]};
        }

        glm::vec3 Transform::backward() const {
            return -forward();
        }

        void Transform::update() {
            *transform = glm::translate(glm::mat4(), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(), scale);
        }

    }
}