#pragma once

#include "entity.h"

#include <memory>
#include <vector>

namespace stirling {

    struct World {
        World();

        void addEntity(Entity* entity);
        void update(float delta_seconds);

    private:
        static const size_t MAX_NUMBER_OF_ENTITIES = 65536;

        Entity* m_entities[MAX_NUMBER_OF_ENTITIES];
		size_t  m_number_of_entities;
    };

}