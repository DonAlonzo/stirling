#pragma once

#include "vulkan/transform.h"

namespace stirling { class Component; }

#include <vector>

namespace stirling {
    class Entity {
    public:
        Entity();
        Entity(vulkan::Transform&& transform);

        vulkan::Transform& transform();

        void addComponent(Component* component);

        virtual void update(float delta_seconds);

    protected:
        static const int MAX_NUMBER_OF_COMPONENTS = 8;

        vulkan::Transform m_transform;
        Component*        m_components[MAX_NUMBER_OF_COMPONENTS];
        int               m_number_of_components = 0;
    };
}