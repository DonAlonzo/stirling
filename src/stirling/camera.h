#pragma once

#include "glm/glm.hpp"

namespace stirling {
    class Camera {
    public:
        Camera();

        void moveTo(const glm::vec3& position);
        void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 0.0f, 1.0f));

        operator const glm::mat4&() const;

    private:
        glm::mat4 m_view_matrix;
    };
}