#pragma once

#include "glm/glm.hpp"

#include "entity.h"

namespace stirling {
    class Camera : public Entity {
    public:
        Camera(float field_of_view, float aspect_ratio, float near_plane, float far_plane);

        void setAspectRatio(float aspect_ratio);
        void setFieldOfView(float field_of_view);
        void setNearPlane(float near_plane);
        void setFarPlane(float far_plane);

        void addComponent(Component* component) = delete;

        const glm::mat4& getProjectionMatrix() const;

		void rotate(float angle, const glm::vec3& axis);
		void moveTo(const glm::vec3& position);
		void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 0.0f, 1.0f));

        void update(float delta_seconds) override;

    private:
		glm::vec3 m_position;
		glm::quat m_rotation;
        glm::mat4 m_projection_matrix;
        float     m_aspect_ratio;
        float     m_field_of_view;
        float     m_near_plane;
        float     m_far_plane;

        void updateProjectionMatrix();
    };
}