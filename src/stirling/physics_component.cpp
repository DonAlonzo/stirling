#include "physics_component.h"
#include "entity.h"

#include <iostream>

namespace stirling {

    void PhysicsComponent::update(Entity* entity, float delta_seconds) {
        entity->transform.rotation *= glm::angleAxis(delta_seconds * .02f, glm::vec3(0.f, 0.f, 1.f));
    }

}