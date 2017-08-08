#pragma once

#include "transform.h"

namespace stirling { struct Component; }

#include <vector>

namespace stirling {

    struct Entity {
        Entity();
        Entity(vulkan::Transform&& transform);

        vulkan::Transform& transform();

        void addComponent(Component* component);

        virtual void update(float delta_seconds);

    protected:
        static constexpr int MAX_NUMBER_OF_COMPONENTS = 8;

        vulkan::Transform transform;
        Component*        components[MAX_NUMBER_OF_COMPONENTS];
        int               number_of_components = 0;
    };

}