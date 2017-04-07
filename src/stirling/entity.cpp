#include "entity.h"

namespace stirling {

    vulkan::Transform& Entity::transform() {
        return m_transform;
    }

    void Entity::update(float delta_seconds) {
    }

}