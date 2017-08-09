#include "entity.h"

#include "component.h"

#include <algorithm>

namespace stirling {

    Entity::Entity() {
    }

    Entity::Entity(vulkan::Transform&& transform) :
        transform (std::move(transform)) {
    }

    void Entity::addComponent(Component* component) {
        assert(number_of_components < MAX_NUMBER_OF_COMPONENTS);
        components[number_of_components++] = component;
    }

    void Entity::update(float delta_seconds) {
        transform.update();
        for (int i = 0; i < number_of_components; ++i) {
            components[i]->update(this, delta_seconds);
        }
    }

}