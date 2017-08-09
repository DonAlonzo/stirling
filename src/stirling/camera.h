#pragma once

#include "glm/glm.hpp"

#include "entity.h"

namespace stirling {

    struct Camera : Entity {
		glm::vec3 position;
        glm::mat4 projection_matrix;
        float pitch = 0.f;
        float yaw   = 0.;
        float roll  = 0.f;

        Camera(float field_of_view, float aspect_ratio, float near_plane, float far_plane);

        void setAspectRatio(float aspect_ratio);
        void setFieldOfView(float field_of_view);
        void setNearPlane(float near_plane);
        void setFarPlane(float far_plane);

        void addComponent(Component* component) = delete;

        void lookAt(const glm::vec3& target, const glm::vec3& up = { 0.f, 0.f, 1.f });

        void update(float delta_seconds) override;

    private:
		glm::quat rotation;

        float aspect_ratio;
        float field_of_view;
        float near_plane;
        float far_plane;

        void updateProjectionMatrix();
    };

}