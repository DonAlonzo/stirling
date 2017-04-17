#pragma once

#include "entity.h"

#include <memory>
#include <vector>

namespace stirling {
    class World {
    public:
        World();

        void addEntity(Entity* entity);

        void update(float delta_seconds);

    private:
        static const int MAX_NUMBER_OF_ENTITIES = 128;

        Entity* m_entities[MAX_NUMBER_OF_ENTITIES];
        int     m_number_of_entities;
    };
}