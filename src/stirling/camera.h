#pragma once

#include "glm/glm.hpp"

#include "entity.h"

namespace stirling {

    struct Camera : Entity {
		glm::vec3 position;
		glm::quat rotation;
        glm::mat4 projection_matrix;

        Camera(float field_of_view, float aspect_ratio, float near_plane, float far_plane);

        void setAspectRatio(float aspect_ratio);
        void setFieldOfView(float field_of_view);
        void setNearPlane(float near_plane);
        void setFarPlane(float far_plane);

        void addComponent(Component* component) = delete;

		void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 0.0f, 1.0f));

        void update(float delta_seconds) override;

    private:
        float aspect_ratio;
        float field_of_view;
        float near_plane;
        float far_plane;

        void updateProjectionMatrix();
    };

}