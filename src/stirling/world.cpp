#include "world.h"

namespace stirling {

    void World::addEntity(std::shared_ptr<Entity> entity) {
        m_entities.push_back(entity);
    }

    void World::update(float delta_seconds) {
        for (auto& entity : m_entities) {
            entity->update(delta_seconds);
        }
    }

}