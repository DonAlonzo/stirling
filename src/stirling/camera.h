#pragma once

#include "glm/glm.hpp"

#include "vulkan/transform.h"

namespace stirling {
    class Camera : public vulkan::Transform {
    public:
        Camera(float field_of_view, float aspect_ratio, float near_plane, float far_plane);

        void setAspectRatio(float aspect_ratio);
        void setFieldOfView(float field_of_view);
        void setNearPlane(float near_plane);
        void setFarPlane(float far_plane);

        const glm::mat4& getProjectionMatrix() const;
        
    private:
        glm::mat4 m_projection_matrix;
        float     m_aspect_ratio;
        float     m_field_of_view;
        float     m_near_plane;
        float     m_far_plane;

        void updateProjectionMatrix();
    };
}