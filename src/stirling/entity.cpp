#include "entity.h"

#include "component.h"

namespace stirling {

    Entity::Entity() :
        m_number_of_components(0) {
    }

    vulkan::Transform& Entity::transform() {
        return m_transform;
    }

    void Entity::addComponent(Component* component) {
        assert(m_number_of_components < MAX_NUMBER_OF_COMPONENTS);
        m_components[m_number_of_components++] = component;
    }

    void Entity::update(float delta_seconds) {
        for (int i = 0; i < m_number_of_components; ++i) {
            m_components[i]->update(this, delta_seconds);
        }
    }

}