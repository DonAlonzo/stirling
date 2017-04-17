#include "world.h"

namespace stirling {

    World::World() :
        m_number_of_entities(0) {
    }

    void World::addEntity(Entity* entity) {
        assert(m_number_of_entities < MAX_NUMBER_OF_ENTITIES);
        m_entities[m_number_of_entities++] = entity;
    }

    void World::update(float delta_seconds) {
        for (int i = 0; i < m_number_of_entities; ++i) {
            m_entities[i]->update(delta_seconds);
        }
    }

}