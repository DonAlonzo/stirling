#pragma once

#include "vulkan/transform.h"

namespace stirling {
    class Entity {
    public:
        vulkan::Transform& transform();

        virtual void update(float delta_seconds);

    protected:
        vulkan::Transform m_transform;
    };
}