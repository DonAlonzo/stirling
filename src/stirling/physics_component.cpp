#include "physics_component.h"
#include "entity.h"

#include <iostream>

namespace stirling {

    void PhysicsComponent::update(Entity* entity, float delta_seconds) {
        entity->transform.rotate(delta_seconds * 0.02f, glm::vec3(0.0f, 0.0f, 1.0f));
    }

}